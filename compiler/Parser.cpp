#include "Parser.h"
#include "AST.h"
#include "Constructs/Annotation.h"

#include <assert.h>

#define DEBUG_PARSING 0

namespace Language {
    Parser::Parser(Lexer* lexer) : _lexer(lexer) {
        _currentScope = Scope::createRootScope();
        _rootModule = Module::createRootModule();
    }

    Parser::~Parser() {
        // delete _currentScope;
    }

    RootNode* Parser::rootASTNode() {
        RootNode* root;

        root = new RootNode();

        while (!this->peek().isEnd()) {
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

        this->parseNewline(true);
        node->setStatement(true);

        return node;
    }

    ASTNode* Parser::parsePrimary() {
#if DEBUG_PARSING
        std::cout << "Parser: primary: '" << this->peek().str() << "'" << std::endl;
#endif

        switch (this->peek().type()) {
            case Token::Type::PunctuationOpenParen: {
                this->next();
                ASTNode* node = this->parseExpression();

                assert(this->next().type() == Token::Type::PunctuationCloseParen);
                return node;
            }
            case Token::Type::Identifier:
                if (this->peek(2).type() == Token::Type::PunctuationOpenParen) {
                    return FunctionCallNode::parse(*this);
                } else {
                    return VariableNode::parse(*this);
                }
                break;
            case Token::Type::KeywordClosure:
                return ClosureNode::parse(*this);
            case Token::Type::String:
                return StringLiteralNode::parse(*this);
            case Token::Type::NumericLiteral:
                return IntegerLiteralNode::parse(*this);
            case Token::Type::TrueLiteral:
            case Token::Type::FalseLiteral:
                return BooleanLiteralNode::parse(*this);
            case Token::Type::NullLiteral:
                return NullLiteralNode::parse(*this);
            case Token::Type::KeywordAtomic:
                return AtomicExpressionNode::parse(*this);
            default:
                break;
        }

        return NULL;
    }

    ASTNode* Parser::parseExpression() {
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: expression: '" << this->peek().str() << "'" << std::endl;
#endif

        node = this->parsePrimary();

        if (!node) {
            // perhaps we have a unary operator?
            node = OperatorNode::parseUnary(*this);
        }

        if (!node) {
            std::cout << "Parser: unhandled expression '" << this->next().str() << "'" << std::endl;
        }
        assert(node != NULL);

        return OperatorNode::parse(*this, Token::MinimumPrecedence, node);
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

    bool Parser::parseUntilEnd(std::function<void (void)> func) {
        while (this->peek().type() != Token::Type::KeywordEnd) {
            func();
        }

        assert(this->next().type() == Token::Type::KeywordEnd);

        return true;
    }

    bool Parser::isAtType() {
        // Possible forms of variable declarations are:
        // [*]Identifier identifier [= <expression>]
        // [*]{...closure type..} identifier [= <expression>]
        // [*](function type) identifier [= <expression>]
        switch (this->peek().type()) {
            case Token::Type::Operator:
            case Token::Type::Identifier:
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
                break;
            default:
                // if the character isn't one of those, we definitely do not have a type
                return false;
        }

        int peekDepth = 1;

        // We might have a pointer type.  We have to check that, following
        // the '*'s, we have "Identifier identifier"
        while (this->peek(peekDepth).str() == "*") {
            peekDepth += 1;
        }

        Token::Type closingPuntuation;

        switch (this->peek(peekDepth).type()) {
            case Token::Type::Identifier:
                // check if we match the "[*]Identifier identifier" pattern
                return this->peek(peekDepth+1).type() == Token::Type::Identifier;
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

        // parse '*', if they are there
        while (this->nextIf("*")) {
            depth++;
        }

        // parse the type
        switch (this->peek().type()) {
            case Token::Type::Identifier:
                dataType = this->currentModule()->dataTypeForName(this->next().str());
                break;
            case Token::Type::PunctuationOpenBrace:
                return this->parseFunctionType(depth, NULL, NULL);
                break;
            default:
                assert(0 && "Got to a weird value parsing a type");
                break;
        }

        assert(dataType);

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

            // we might have a parameter (not for a function type)
            if (this->peek().type() == Token::Type::Identifier) {
                assert(params);
                params->push_back(this->next().str());
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

    Module* Parser::moduleWithIdentifier(const std::string& name) {
        Module* module = _module[name];

        if (!module) {
            module = Module::createModule(_rootModule, name, std::vector<std::string>());
            _rootModule->addChild(module);
        }

        return module;
    }

    Module* Parser::currentModule() const {
        return _rootModule;
    }

    ASTNode* Parser::parseTopLevelNode() {
        ASTNode* node = NULL;
        // std::cout << "Parser: top level" << std::endl;
        
        switch (this->peek().type()) {
            case Token::Type::KeywordDef:
                return this->parseDefinition();
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
            case Token::Type::EndOfInput:
                assert(0 && "parseTopLevelNode invalid state");
                break;
            default:
                this->next();
                return new RootNode();
        }

        return NULL;
    }

    ASTNode* Parser::parseDefinition() {
        // std::cout << "Parser: definition" << std::endl;

        // the kinds of definitions we have are:
        // - a function body
        // - a function prototype
        // - a type

        assert(this->next().type() == Token::Type::KeywordDef); // skip over the "def"

        if (this->peek(2).str() == "(") {
            return FunctionDefinitionNode::parse(*this);
        }

        return new RootNode();
    }

    Token Parser::peek(unsigned int distance) {
        return _lexer->peek(distance);
    }

    Token Parser::next() {
        return _lexer->next();
    }

    bool Parser::nextIf(const std::string& value) {
        return _lexer->nextIf(value);
    }

    Scope* Parser::currentScope() const {
        assert(_currentScope);

        return _currentScope;
    }

    void Parser::pushScope(Scope* scope) {
        assert(scope);

        scope->setParent(this->currentScope());
        _currentScope = scope;
    }

    void Parser::popScope() {
        Scope* old = this->currentScope();

        _currentScope = old->parent();
        assert(_currentScope);

        delete old;
    }
}
