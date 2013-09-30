#include "Parser.h"
#include "AST.h"
#include "Constructs/Annotation.h"

#include <assert.h>

#define DEBUG_PARSING 0

namespace Language {
    Parser::Parser(Lexer* lexer) : _lexer(lexer) {
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
                node = Three::ForNode::parse(*this);
                break;
            case Token::Type::KeywordBreak:
                node = Three::BreakNode::parse(*this);
                break;
            case Token::Type::KeywordContinue:
                node = Three::ContinueNode::parse(*this);
                break;
            case Token::Type::KeywordSwitch:
                node = SwitchNode::parse(*this);
                break;
            case Token::Type::KeywordAtomic:
                node = AtomicStatementNode::parse(*this);
                break;
            case Token::Type::KeywordAbort:
                node = AbortStatementNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            case Token::Type::KeywordBarrier:
                node = BarrierNode::parse(*this);
                node = IfNode::parseTailing(*this, node);
                break;
            case Token::Type::Annotation:
                node = AnnotationNode::parse(*this);
                break;
            default:
                break;
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
            node = IfNode::parseTailing(*this, node);
        }

#if DEBUG_PARSING
            std::cout << "Parser: statement complete '"  << this->peek().str() << "'" << std::endl;
#endif

        this->parseNewline();
        node->setStatement(true);

        return node;
    }

    ASTNode* Parser::parsePrimaryExpression() {
        ASTNode* node = this->parseSecondaryExpression();

        // we now need to check for a tailing operator
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
            case Token::Type::String:
                return StringLiteralNode::parse(*this);
            case Token::Type::NumericLiteral:
                return IntegerLiteralNode::parse(*this);
            case Token::Type::PunctuationOpenParen: {
                this->next();
                ASTNode* node = this->parseExpression();

                assert(this->next().type() == Token::Type::PunctuationCloseParen);
                return node;
            }
            case Token::Type::TrueLiteral:
            case Token::Type::FalseLiteral:
                return BooleanLiteralNode::parse(*this);
            case Token::Type::NullLiteral:
                return NullLiteralNode::parse(*this);
            case Token::Type::KeywordAtomic:
                return AtomicExpressionNode::parse(*this);
            case Token::Type::KeywordSizeof:
                return Three::SizeofNode::parse(*this);
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
            if (t.isStatementEnding() || t.type() != Token::Type::Operator) {
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
        // possible function call, variable, or constant

        std::string identifier = this->parseQualifiedIdentifier();

        if (this->peek().type() == Token::Type::PunctuationOpenParen) {
            return FunctionCallNode::parse(*this, identifier, NULL);
        }

        if (this->currentModule()->definesConstant(identifier)) {
            return new Three::ValueNode(identifier);
        }

        return VariableNode::parse(*this, identifier);
    }

    bool Parser::parseNewline(bool multipleAllowed) {
        while (true) {
            assert(this->next().type() == Token::Type::Newline);
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

    std::string Parser::parseQualifiedIdentifier() {
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

            // this is a bit of hack to get namespaces work in a way that is
            // identifier-compatible with C
            s << "_3_" << this->next().str();
        }

        return s.str();
    }

    bool Parser::isAtType() {
        // Possible forms of variable declarations are:
        // QualifiedIdentifier identifier = <expression>
        // QualifiedIdentifier:<number> identifier = <expression>
        // {...closure type..} identifier = <expression>
        // (function type) identifier = <expression>
        //
        // and each of these can be preceended by arbitrary pointers, arrays, and annotations
        switch (this->peek().type()) {
            case Token::Type::Operator:
            case Token::Type::Identifier:
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
            case Token::Type::Annotation:
                break;
            default:
                // if the character isn't one of those, we definitely do not have a type
                return false;
        }

        int peekDepth = 1;

        // Pointers, arrays, annotations could be intermixed.
        while (1) {
            Token t = this->peek(peekDepth);

            if (t.str() == "*") {
                peekDepth += 1;
                continue;
            } else if (t.type() == Token::Type::Annotation) {
                peekDepth += 1;
                continue;
            } else if (t.str() == "[") {
                peekDepth += 1;

                if (this->peek(peekDepth).type() != Token::Type::NumericLiteral) {
                    return false;
                }
                peekDepth += 1;

                if (this->peek(peekDepth).type() != Token::Type::PunctuationCloseBracket) {
                    return false;
                }
                peekDepth += 1;

                continue;
            }

            break; // finish the loop
        }

        // check if we match:
        // - "QualifiedIdentifier identifier"
        // - "QualifiedIdentifier:<number> identifier"
        while (true) {
            if (this->peek(peekDepth).type() != Token::Type::Identifier) {
                break;
            }

            if (this->peek(peekDepth+1).type() != Token::Type::OperatorScope) {
                break;
            }

            peekDepth += 2;
        }

        Token::Type closingPuntuation;

        switch (this->peek(peekDepth).type()) {
            case Token::Type::Identifier:
                // check if we match the "*Identifier identifier" pattern
                if (this->peek(peekDepth+1).type() == Token::Type::Identifier) {
                    return true;
                }

                // check if we match the "*Identifier:<number> identifier" pattern
                if (this->peek(peekDepth+1).type() != Token::Type::PunctuationColon) {
                    return false;
                }

                if (this->peek(peekDepth+2).type() != Token::Type::NumericLiteral) {
                    return false;
                }

                return this->peek(peekDepth+3).type() == Token::Type::Identifier;
            case Token::Type::PunctuationOpenParen:
                closingPuntuation = Token::Type::PunctuationCloseParen;
                break;
            case Token::Type::PunctuationOpenBrace:
                closingPuntuation = Token::Type::PunctuationCloseBrace;
                break;
            default:
                return false;
        }

        // Ok, still need to check for closure and function types

        // "{} Identifier"
        // "() Identifier"
        peekDepth += 1;

        if (this->peek(peekDepth).type() == closingPuntuation) {
            return this->peek(peekDepth+1).type() == Token::Type::Identifier;
        }

        // "{Identifier...} Identifier"
        // "{;...} Identifier"
        switch (this->peek(peekDepth).type()) {
            case Token::Type::Identifier:
            case Token::Type::PunctuationSemicolon:
                return true;
            default:
                break;
        }

        return false;
    }

    TypeReference Parser::parseType() {
        DataType* dataType = NULL;
        uint32_t  depth    = 0;

#if DEBUG_PARSING
        std::cout << "Parser: type: '" << this->peek().str() << "'" << std::endl;
#endif

        // parse '*' and annotations, if they are there
        while (1) {
            if (this->peek().str() == "*") {
                this->next();
                depth++;
                continue;
            }

            if (this->peek().type() == Token::Type::Annotation) {
                this->next();
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
            case Token::Type::PunctuationOpenBrace:
            case Token::Type::PunctuationOpenParen:
                return this->parseFunctionType(depth, NULL, NULL);
                break;
            default:
                assert(0 && "Got to a weird value parsing a type");
                break;
        }

        assert(dataType && "Unable to create type");

        return TypeReference(dataType, depth);
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

            type->addChild(this->parseType());

            // We might have a parameter label.  If the caller cares, we need to enforce that a label is present.
            if (params) {
                assert(this->peek().type() == Token::Type::Identifier);
                params->push_back(this->next().str());
            } else {
                if (this->peek().type() == Token::Type::Identifier) {
                    this->next();
                }
            }

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
                return Three::DefinitionNode::parse(*this, false);
            case Token::Type::KeywordImport:
                return ImportNode::parse(*this);
            case Token::Type::KeywordStructure:
                return StructureNode::parse(*this);
            case Token::Type::KeywordEnumeration:
                return EnumerationNode::parse(*this);
            case Token::Type::Annotation:
                node = AnnotationNode::parse(*this);
                this->parseNewline();
                return node;
            case Token::Type::KeywordLinkage:
                return Three::LinkageNode::parse(*this);
            case Token::Type::KeywordInclude:
                return Three::IncludeNode::parse(*this);
            case Token::Type::KeywordModule:
                return Three::ModuleNode::parse(*this);
            case Token::Type::KeywordPublic:
            case Token::Type::KeywordPrivate:
                return Three::VisibilityNode::parse(*this);
            case Token::Type::KeywordNamespace:
                return Three::NamespaceNode::parse(*this);
            case Token::Type::EndOfInput:
                assert(0 && "parseTopLevelNode invalid state");
                break;
            default:
                break;
        }

        if (this->isAtType()) {
            node = VariableDeclarationNode::parse(*this);
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

    Three::ParsingContext* Parser::context() const {
        return _context;
    }

    void Parser::setContext(Three::ParsingContext* context) {
        _context = context;
    }

    TranslationUnit* Parser::currentTranslationUnit() const {
        return _context->translationUnit();
    }

    Three::Module* Parser::currentModule() const {
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
