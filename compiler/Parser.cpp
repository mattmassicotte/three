#include "Parser.h"
#include "AST.h"
#include "Constructs/Annotation.h"
#include "Constructs/Function.h"
#include "Constructs/Scope.h"
#include "Constructs/TranslationUnit.h"
#include "Constructs/TypeReference.h"
#include "Preprocessing/Preprocessor.h"

#include <assert.h>
#include <iostream>
#include <fstream>

#define DEBUG_PARSING 0

namespace Three {
    ParsingContext* Parser::contextFromFile(const std::string& path) {
        ParsingContext* context = ParsingContext::translationUnitContext();

        std::ifstream inputFile(path);

        std::string processedString(Preprocessor::process(&inputFile));

        std::istringstream inputString(processedString);

        NewLexer lexer(&inputString);
        Parser parser(&lexer);

        parser.parse(context);

        return context;
    }

    Parser::Parser(NewLexer* lexer) : _lexer(lexer) {
    }

    Parser::~Parser() {
        // delete _currentScope;
    }

    RootNode* Parser::rootASTNode() {
        RootNode* root;

        root = new RootNode();

        while (!this->peek().isEnd()) {
#if DEBUG_PARSING
            std::cout << "Parser: root '" << this->peek().str() << "'" << std::endl;
#endif
            root->addChild(this->parseTopLevelNode());
        }

        return root;
    }

    bool Parser::parse(ParsingContext* context) {
        assert(context);
        _context = context;

        _context->setRootNode(this->rootASTNode());

        return true;
    }

    ASTNode* Parser::parseStatement() {
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: statement '" << this->peek().str() << "'" << std::endl;
#endif

        // Checking for tailing ifs is a little tricky here.  Certain things
        // can have them, certain ones cannot.
        switch (this->peek().type()) {
            case Token::Type::KeywordReturn:
                node = ReturnNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            case Token::Type::KeywordIf:
                node = IfNode::parse(*this);
                break;
            case Token::Type::KeywordLoop:
                node = LoopNode::parse(*this);
                break;
            case Token::Type::KeywordFor:
                node = ForNode::parse(*this);
                break;
            case Token::Type::KeywordBreak:
                node = BreakNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            case Token::Type::KeywordContinue:
                node = ContinueNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            case Token::Type::KeywordSwitch:
                node = SwitchNode::parse(*this);
                break;
            case Token::Type::KeywordAtomic:
                node = AtomicNode::parse(*this, true);
                break;
            case Token::Type::KeywordAbort:
                node = AbortStatementNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            case Token::Type::KeywordBarrier:
                node = BarrierNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            default:
                break;
        }

        if (!node && this->peek().isAnnotation()) {
            node = AnnotationNode::parse(*this);
        }

        // Next, check for a possible variable declaration
        if (!node && this->isAtType()) {
            node = VariableDeclarationNode::parse(*this);
        }

        if (!node) {
#if DEBUG_PARSING
            std::cout << "Parser: falling back to expression statement" << std::endl;
#endif
            node = this->parseExpression();
            node->setStatement(true);

            node = IfNode::parseTailing(*this, node);
        }

#if DEBUG_PARSING
            std::cout << "Parser: statement complete '"  << this->peek().str() << "'" << std::endl;
#endif

        this->parseNewline();

        return node;
    }

    ASTNode* Parser::parsePrimaryExpression() {
        ASTNode* node = this->parseSecondaryExpression();

        // We now need to check for a tailing operator
        return OperatorNode::parseTailing(*this, node);
    }
    
    ASTNode* Parser::parseSecondaryExpression() {
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: parseSecondaryExpression: '" << this->peek().str() << "'" << std::endl;
#endif

        // try a unary operator first
        if (this->peek().isUnaryOperator()) {
#if DEBUG_PARSING
            std::cout << "Parser: unary in parseSecondaryExpression: '" << this->peek().str() << "'" << std::endl;
#endif
            return OperatorNode::parseUnary(*this);
        }

        switch (this->peek().type()) {
            case Token::Type::Identifier:
                return this->parseSecondaryIdentifier();
            case Token::Type::KeywordClosure:
                return ClosureNode::parse(*this);
            case Token::Type::LiteralString:
                return StringLiteralNode::parse(*this);
            case Token::Type::LiteralInteger:
            case Token::Type::LiteralHex:
            case Token::Type::LiteralBinary:
                return IntegerLiteralNode::parse(*this);
            case Token::Type::PunctuationOpenParen: {
                this->next();
                ASTNode* node = this->parseExpression();

                assert(this->next().type() == Token::Type::PunctuationCloseParen);
                return node;
            }
            case Token::Type::LiteralTrue:
            case Token::Type::LiteralFalse:
                return BooleanLiteralNode::parse(*this);
            case Token::Type::LiteralNull:
                return NullLiteralNode::parse(*this);
            case Token::Type::KeywordAtomic:
                return AtomicExpressionNode::parse(*this, false);
            case Token::Type::MetafunctionSizeOf:
                return SizeofNode::parse(*this);
            case Token::Type::MetafunctionCast:
                return CastNode::parse(*this);
            case Token::Type::MetafunctionNextarg:
                return NextargNode::parse(*this);
            default:
                break;
        }

        return NULL;
    }

    ASTNode* Parser::parseExpression(uint32_t precedence) {
        // Parsing expressions is very complex, and depends on correctly setting
        // the precedence and associativity of operator tokens.
        // The algorithm used is called Precedence Climbing.
        assert(precedence > Token::NonPrecedence);

#if DEBUG_PARSING
        std::cout << "Parser: parseExpression (" << precedence << ")" << std::endl;
#endif

        ASTNode* left = this->parsePrimaryExpression();

        while (true) {
            Token t = this->peek();

            // if we've gotten to the end of a statement, or do not have an operator
            if (t.isStatementEnding() || !t.isOperator()) {
                break;
            }

            // ok we have a binary operator token
            uint32_t nextPrecedence = t.precedence();
            assert(nextPrecedence != Token::NonPrecedence);

            if (nextPrecedence < precedence) {
#if DEBUG_PARSING
                std::cout << "Parser: parseExpression lower " << t.str() << " (" << nextPrecedence << ")" << std::endl;
#endif
                break;
            }

            if (t.isLeftAssociative()) {
                nextPrecedence = nextPrecedence + 1;
            }

#if DEBUG_PARSING
            std::cout << "Parser: parseExpression " << t.str() << " : " << nextPrecedence << " compare " << precedence << std::endl;
#endif

            left = OperatorNode::parse(*this, left, nextPrecedence);
        }

        return left;
    }

    ASTNode* Parser::parseSecondaryIdentifier() {
#if DEBUG_PARSING
        std::cout << "Parser: parseSecondaryIdentifier: '" << this->peek().str() << "'" << std::endl;
#endif
        // possible variable or constant

        std::string identifier = this->parseQualifiedIdentifier();

        if (this->currentModule()->definesConstant(identifier)) {
            // TODO: Need to know the type of this node
            TypeReference type = TypeReference::ref(this->currentModule(), "Void", 0);
            ASTNode* value = new ValueNode(identifier, type);

            // Ok, this part is a little on the awful side. I cannot figure out how to 
            // differentiate between value macros and function-like macros. So, we need to determine
            // how they are being used in code.
            if (this->peek().type() != Token::Type::PunctuationOpenParen) {
                return value;
            }

            return OperatorNode::parseTailing(*this, value);
        }

        return VariableNode::parse(*this, identifier);
    }

    bool Parser::parseNewline(bool multipleAllowed) {
        while (true) {
            if (this->peek().type() != Token::Type::Newline) {
                std::cout << "[Parser] expecting newline, found '" << this->peek().str() << "'" << std::endl;
                assert(0 && "Failed to parse expected newline");
            }

            this->next();

            if (!multipleAllowed) {
                break;
            }
            
            if (this->peek().type() != Token::Type::Newline) {
                break;
            }
        }

        return true;
    }

    bool Parser::parseUntil(bool advanceOnStop, std::function<bool (const Token& token)> func) {
        do {
            Token t = this->peek();

            if (t.type() == Token::Type::EndOfInput) {
                return false;
            }

            if (func(t)) {
                break;
            }

            // TODO: this is an infinite loop if func does not call next at least once.
        } while (1);

        if (advanceOnStop) {
            this->next();
        }

        return true;
    }

    bool Parser::parseUntilEnd(std::function<void (void)> func) {
        return this->parseUntil(true, [&] (const Token& token) {
            if (token.type() != Token::Type::KeywordEnd) {
                func();
                return false;
            }

            return true;
        });
    }

    std::string Parser::parseQualifiedIdentifier(const std::string& seperator) {
        std::stringstream s;

        // Identifier::Identifier...

        // we must have at least one identifier
        assert(this->peek().type() == Token::Type::Identifier);
        s << this->next().str();

        while (true) {
            // if we hit a scope operator, keep going
            if (this->peek().type() != Token::Type::OperatorScope) {
                break;
            }

            this->next();
            assert(this->peek().type() == Token::Type::Identifier);

            s << seperator << this->next().str();
        }

        return s.str();
    }

    bool Parser::peekTypePrefixes(unsigned int* peekDepth) {
        unsigned int originalDepth = *peekDepth;

        // Pointers, arrays, annotations could be intermixed.
        for (;;) {
            Token t = this->peek(*peekDepth);

            if (t.str() == "*") {
                *peekDepth += 1;
                continue;
            } else if (t.isAnnotation()) {
                *peekDepth += 1;
                continue;
            } else if (t.str() == "[") {
                *peekDepth += 1;

                if (this->peek(*peekDepth).type() != Token::Type::LiteralInteger) {
                    return false;
                }
                *peekDepth += 1;

                if (this->peek(*peekDepth).type() != Token::Type::PunctuationCloseBracket) {
                    return false;
                }
                *peekDepth += 1;

                continue;
            }

            break; // finish the loop
        }

        // return true if we've advanced at all
        return *peekDepth > originalDepth;
    }

    bool Parser::peekNonFunctionType(unsigned int* peekDepth) {
        unsigned int originalDepth = *peekDepth;

        // move past any type prefixes
        this->peekTypePrefixes(peekDepth);

#if DEBUG_PARSING
        std::cout << "[Parser] peekNonFunctionType: " << this->peek(*peekDepth).str() << std::endl;
#endif

        // We have to lead with an identifier
        if (this->peek(*peekDepth).type() != Token::Type::Identifier) {
            return false;
        }

        *peekDepth += 1;

        // ::Identifier
        for (;;) {
            if (this->peek(*peekDepth).type() != Token::Type::OperatorScope) {
                break;
            }

            if (this->peek(*peekDepth+1).type() != Token::Type::Identifier) {
                break;
            }

            *peekDepth += 2;
        }

        // :specifier
        for (;;) {
            if (this->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
                break;
            }

            if (this->peek(*peekDepth+1).type() == Token::Type::LiteralInteger) {
                *peekDepth += 2;
                continue;
            }

            // if we get here, we definitely don't have a specified type
            return false;
        }

        return true;
    }

    bool Parser::peekFunctionType(unsigned int* peekDepth) {
        Token::Type closingPunctuation;

        // move past any type prefixes
        this->peekTypePrefixes(peekDepth);

#if DEBUG_PARSING
        std::cout << "[Parser] peekFunctionType: " << this->peek(*peekDepth).str() << std::endl;
#endif

        // "{} Identifier"
        // "() Identifier"
        // "{; Type} Identifier"
        // "(; Type) Identifier"
        switch (this->peek(*peekDepth).type()) {
            case Token::Type::PunctuationOpenParen:
                closingPunctuation = Token::Type::PunctuationCloseParen;
                break;
            case Token::Type::PunctuationOpenBrace:
                closingPunctuation = Token::Type::PunctuationCloseBrace;
                break;
            default:
                return false;
        }

        *peekDepth += 1;

        // This part is tricky... we need to advance past the parameters
        while (this->peekType(peekDepth)) {
            if (this->peek(*peekDepth).str() == ",") {
                *peekDepth += 1;
            }
        }

#if DEBUG_PARSING
        std::cout << "[Parser] peekFunctionType return: " << this->peek(*peekDepth).str() << std::endl;
#endif

        // here, we might have a return value
        if (this->peek(*peekDepth).type() == Token::Type::PunctuationSemicolon) {
            *peekDepth += 1;

            // make sure the return type is specified
            if (!this->peekType(peekDepth)) {
                return false;
            }
        }

        // now, just check to make sure the closing punctuation is correction
        if (this->peek(*peekDepth).type() == closingPunctuation) {
            *peekDepth += 1;

            return true;
        }

        return false;
    }

    bool Parser::peekType(unsigned int* peekDepth) {
#if DEBUG_PARSING
        std::cout << "[Parser] peek type: " << this->peek().str() << std::endl;
#endif

        if (this->peekNonFunctionType(peekDepth)) {
#if DEBUG_PARSING
            std::cout << "[Parser] peek type found non-function type" << std::endl;
#endif
            return true;
        }

        if (this->peekFunctionType(peekDepth)) {
#if DEBUG_PARSING
            std::cout << "[Parser] peek type found function type" << std::endl;
#endif
            return true;
        }

        return false;
    }

    bool Parser::isAtType() {
        unsigned int peekDepth = 1;

        // This function is really checking for a variable declaration, not just a type.  Variable
        // declarations always have the following form:
        // <Type> identifier
        if (!this->peekType(&peekDepth)) {
            return false;
        }

        // here, peekDepth has been advanced past the type
#if DEBUG_PARSING
        std::cout << "[Parser] Possible variable declaration found: " << this->peek(peekDepth).str() << std::endl;
#endif

        return this->peek(peekDepth).type() == Token::Type::Identifier;
    }

    TypeReference Parser::parseType() {
        DataType* dataType = NULL;
        uint32_t  depth    = 0;
        std::vector<uint32_t> dimensions;

#if DEBUG_PARSING
        std::cout << "Parser: type: '" << this->peek().str() << "'" << std::endl;
#endif

        // parse '*', annotations and array dimensions, if they are there
        while (1) {
            if (this->nextIf("*")) {
                depth++;
                continue;
            }

            if (this->peek().isAnnotation()) {
                this->next();
                continue;
            }

            if (this->nextIf("[")) {
                assert(this->peek().type() == Token::Type::LiteralInteger);
                dimensions.push_back(strtol(this->next().str().c_str(), NULL, 10));
                assert(this->next().type() == Token::Type::PunctuationCloseBracket);
                continue;
            }

            break;
        }

        std::string typeName;
        std::string namespacedName;

        // parse the type
        switch (this->peek().type()) {
            case Token::Type::Identifier:
                typeName = this->parseQualifiedIdentifier();

                // parse the specialization
                if (this->peek().type() == Token::Type::PunctuationColon) {
                    typeName += this->next().str();
                    typeName += this->next().str();
                }

                namespacedName = this->currentScope()->fullNamespace() + "_3_" + typeName;

                dataType = this->currentModule()->dataTypeForName(typeName);
                if (!dataType) {
                    dataType = this->currentModule()->dataTypeForName(namespacedName);
                }

                if (!dataType) {
                    std::cout << "[Parser] Unable to up look type '" << typeName << "' or '" << namespacedName << "'" << std::endl;
                }

                break;
            case Token::Type::KeywordVararg:
                this->next();
                dataType = this->currentModule()->dataTypeForName("Vararg");
                break;
            case Token::Type::PunctuationOpenBrace:
            case Token::Type::PunctuationOpenParen:
                assert(dimensions.size() == 0 && "Array of functions/closures not supported yet");
                return this->parseFunctionType(depth, NULL, NULL);
                break;
            default:
                assert(0 && "Got to a weird value parsing a type");
                break;
        }

        assert(dataType && "Unable to create type");

        return TypeReference(dataType, depth, dimensions);
    }

    TypeReference Parser::parseFunctionType(uint32_t depth, std::vector<std::string>* params, std::vector<std::string>* references) {
        DataType* type;

#if DEBUG_PARSING
        std::cout << "Parser: function type: '" << this->peek().str() << "'" << std::endl;
#endif

        // determine if we are parsing a function type, a closure type, or we have an error
        switch (this->peek().str().at(0)) {
            case '(':
                type = new DataType(DataType::Flavor::Function, "");
                break;
            case '{':
                // we know this is a closure type
                type = new DataType(DataType::Flavor::Closure, "");
                break;
            default:
                assert(0 && "Trying to parse a function type, but didn't find an opening '(' or '{'");
                break;
        }

        this->next(); // advance past the opening punctuation

        while (true) {
            Token t = this->peek();

            if (t.str().at(0) == ';' || t.type() == Token::Type::PunctuationCloseParen || t.type() == Token::Type::PunctuationCloseBrace) {
                break;
            }

            std::string label = "";
            TypeReference paramType = this->parseType();

            // We might have a parameter label.  If the caller cares, we need to enforce that a label is present.
            if (this->peek().type() == Token::Type::Identifier) {
                label = this->next().str();
            }

            if (params) {
                assert(label.length() > 0);
                params->push_back(label);
            }

            type->addChild(paramType, label);

            // a ',' means another paramter was specified
            if (!this->nextIf(",")) {
                break;
            }
        }

        // assume that we're going to be returning Void
        type->setReturnType(TypeReference(this->currentModule()->dataTypeForName("Void"), 0));

        // could be just the end
        // - }
        // - )

        if (this->nextIf(")") || this->nextIf("}")) {
            return TypeReference(type, depth);
        }

        // could have a type, followed by end
        // - ;Type)
        // - ;Type)

        assert(this->nextIf(";"));
        type->setReturnType(this->parseType());
        if (this->nextIf(")") || this->nextIf("}")) {
            return TypeReference(type, depth);
        }

        // still have closure capture references
        assert(this->nextIf(";"));
        assert(references);
        type->setFlavor(DataType::Flavor::Closure);

        // loop over the capture list
        while (true) {
            Token t = this->peek();
            if (t.str().at(0) == '}') {
                break;
            }

            assert(t.type() == Token::Type::Identifier);

            references->push_back(t.str());

            this->next();

            if (!this->nextIf(",")) {
                break;
            }
        }

        assert(this->nextIf(")"));

        return TypeReference(type, depth);
    }

    Function* Parser::parseFunctionSignature() {
        Function* function = new Function();

        // TODO: there is some funny-business going on with the function namespace here.
        // The problem is types are namespaced too.  So if we have a method definition,
        // the current namespace gets tacked on twice, once for the function and again for the
        // type.

        // parse the function name
        // name(...)
        // Type.name(...)

        if (this->peek(2).str() == ".") {
            assert(this->peek().type() == Token::Type::Identifier);

            std::string typeName = this->next().str();

            if (this->currentScope()->fullNamespace().length() > 0) {
                typeName = this->currentScope()->fullNamespace() + "_3_" + typeName;
            } else {
                function->setNamespace(this->currentScope()->fullNamespace());
            }

            TypeReference type = TypeReference::ref(this->currentModule(), typeName, 0);

            function->setPseudoMethodType(type);

            type.setIndirectionDepth(1); // turn this into a pointer
            function->addParameter("self", type);

            assert(this->next().str() == ".");
        } else {
            function->setNamespace(this->currentScope()->fullNamespace());
        }

        assert(this->peek().type() == Token::Type::Identifier);
        function->setName(this->next().str());

        // move past the opening '('
        assert(this->next().type() == Token::Type::PunctuationOpenParen);

        // at each point we could have:
        // - a ';'
        // - a Type + identifier, followed by a ';'
        // - a Type + identifier, followed by a ','
        // - a ')'
        while (true) {
            Token t = this->peek();

            if (t.str().at(0) == ';' || t.type() == Token::Type::PunctuationCloseParen) {
                break;
            }

            TypeReference type = this->parseType();

            assert(this->peek().type() == Token::Type::Identifier);
            function->addParameter(this->next().str(), type);

            // a ',' means another paramter was specified
            if (this->nextIf(",")) {
                continue;
            }

            break;
        }

        // now, check for the return type
        if (this->peek().type() == Token::Type::PunctuationSemicolon && this->peek(2).type() != Token::Type::PunctuationCloseParen) {
            // move past the ';'
            assert(this->next().type() == Token::Type::PunctuationSemicolon);

            function->setReturnType(this->parseType());
        } else {
            function->setReturnType(TypeReference::ref(this->currentModule(), "Void", 0));
        }

        // parse the close paren
        assert(this->next().type() == Token::Type::PunctuationCloseParen);

        return function;
    }

    ASTNode* Parser::parseTopLevelNode() {
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: top level '" << this->peek().str() << "'" << std::endl;
#endif
        
        switch (this->peek().type()) {
            case Token::Type::KeywordDef:
                return DefinitionNode::parse(*this, false);
            case Token::Type::KeywordImport:
                return ImportNode::parse(*this);
            case Token::Type::KeywordStructure:
                return StructureNode::parse(*this);
            case Token::Type::KeywordEnumeration:
                return EnumerationNode::parse(*this);
            case Token::Type::KeywordLinkage:
                return LinkageNode::parse(*this);
            case Token::Type::KeywordInclude:
                return IncludeNode::parse(*this);
            case Token::Type::KeywordModule:
                return ModuleNode::parse(*this);
            case Token::Type::KeywordPublic:
            case Token::Type::KeywordPrivate:
                return VisibilityNode::parse(*this);
            case Token::Type::KeywordNamespace:
                return NamespaceNode::parse(*this);
            case Token::Type::EndOfInput:
                assert(0 && "parseTopLevelNode invalid state");
                break;
            default:
                break;
        }

        if (this->peek().isAnnotation()) {
            node = AnnotationNode::parse(*this);
            this->parseNewline();
            return node;
        }

        if (this->isAtType()) {
            // This is a global
            node = VariableDeclarationNode::parse(*this);
            node->setStatement(true);
            dynamic_cast<VariableDeclarationNode*>(node)->global = true; // TODO: this should be improved
            this->parseNewline();

            return node;
        }

        std::cout << "Parser: unhandled top-level token '" << this->next().str() << "'" << std::endl;
        assert(0);

        return NULL;
    }

    Token Parser::peek(unsigned int distance) {
        return _lexer->peek(distance);
    }

    void Parser::peekUntil(unsigned int* distance, std::function<bool (const Token& token)> func) {
        assert(distance);

        bool keepGoing = true;

        while (keepGoing) {
            keepGoing = func(this->peek(*distance));

            if (keepGoing) {
                *distance += 1;
            }
        }
    }

    Token Parser::next() {
        return _lexer->next();
    }

    bool Parser::nextIf(const std::string& value) {
        return _lexer->nextIf(value);
    }

    ParsingContext* Parser::context() const {
        return _context;
    }

    TranslationUnit* Parser::currentTranslationUnit() const {
        return _context->translationUnit();
    }

    Module* Parser::currentModule() const {
        return _context->currentModule();
    }

    Scope* Parser::currentScope() const {
        return _context->currentScope();
    }

    void Parser::pushScope(Scope* scope) {
        _context->pushScope(scope);
    }

    void Parser::popScope() {
        _context->popScope();
    }
}
