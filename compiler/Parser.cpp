#include "Parser.h"
#include "AST.h"

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

        // TODO: This really needs to do more exhausive checks for a variable
        // declaration.

        switch (this->peek().type()) {
            case Token::Type::Identifier:
                if (this->peek(2).type() == Token::Type::Identifier) {
                    node = VariableDeclarationNode::parse(*this);
                }
                break;
            case Token::Type::PunctuationOpenBrace:
                // closure type
                node = VariableDeclarationNode::parse(*this);
                break;
            case Token::Type::KeywordReturn:
                node = ReturnNode::parse(*this);
                break;
            case Token::Type::KeywordIf:
                node = IfNode::parse(*this);
                break;
            default:
                break;
        }

        if (!node) {
#if DEBUG_PARSING
            std::cout << "Parser: falling back to expression statement" << std::endl;
#endif
            node = this->parseExpression();
        }

        this->parseNewline(true);
        node->setStatement(true);

        return node;
    }

    ASTNode* Parser::parseExpression() {
        ASTNode* node = NULL;

#if DEBUG_PARSING
        std::cout << "Parser: expression: '" << this->peek().str() << "'" << std::endl;
#endif

        switch (this->peek().type()) {
            case Token::Type::PunctuationOpenParen:
                this->next();
                node = this->parseExpression();

                assert(this->next().type() == Token::Type::PunctuationCloseParen);
                break;
            case Token::Type::Identifier:
                if (this->peek(2).type() == Token::Type::PunctuationOpenParen) {
                    node = FunctionCallNode::parse(*this);
                } else {
                    node = VariableNode::parse(*this);
                }
                break;
            case Token::Type::KeywordClosure:
                node = ClosureNode::parse(*this);
                break;
            case Token::Type::String:
                node = StringLiteralNode::parse(*this);
                break;
            case Token::Type::NumericLiteral:
                node = IntegerLiteralNode::parse(*this);
                break;
            case Token::Type::TrueLiteral:
            case Token::Type::FalseLiteral:
                node = BooleanLiteralNode::parse(*this);
                break;
            default:
                std::cout << "Parser: unhandled expression '" << this->next().str() << "'" << std::endl;
                break;
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
