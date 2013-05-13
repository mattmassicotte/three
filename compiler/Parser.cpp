#include "Parser.h"
#include "AST/RootNode.h"
#include "AST/FunctionDefinitionNode.h"
#include "AST/FunctionCallNode.h"
#include "AST/StringLiteralNode.h"
#include "AST/ReturnNode.h"
#include "AST/IntegerLiteralNode.h"
#include "AST/IfNode.h"
#include "AST/BooleanLiteralNode.h"

#include <assert.h>

namespace Language {
    Parser::Parser(Lexer* lexer) : _lexer(lexer) {
        _currentScope = Scope::createRootScope();
    }

    Parser::~Parser() {
        delete _currentScope;
    }

    ASTNode* Parser::rootASTNode() {
        RootNode* root;

        root = new RootNode();

        while (!this->peek().isEnd()) {
            root->addChild(this->parseTopLevelNode());
        }

        return root;
    }

    ASTNode* Parser::parseStatement() {
        ASTNode* node = NULL;

        // std::cout << "Parser: statement" << std::endl;

        switch (this->peek().type()) {
            case Token::Type::Identifier:
                if (this->peek(2).str().at(0) == '(') {
                    node = FunctionCallNode::parse(*this);
                }
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
            std::cout << "Parser: unhandled statement '" << this->next().str() << "'" << std::endl;
            node = new RootNode();
        }

        assert(this->next().type() == Token::Type::Newline);

        return node;
    }
    ASTNode* Parser::parseExpression() {
        // std::cout << "Parser: expression" << std::endl;
        switch (this->peek().type()) {
            case Token::Type::String:
                return StringLiteralNode::parse(*this);
            case Token::Type::NumericLiteral:
                return IntegerLiteralNode::parse(*this);
            case Token::Type::TrueLiteral:
            case Token::Type::FalseLiteral:
                return BooleanLiteralNode::parse(*this);
            default:
                std::cout << "Parser: unhandled expression '" << this->next().str() << "'" << std::endl;
                break;
        }

        return new RootNode();
    }

    ASTNode* Parser::parseTopLevelNode() {
        // std::cout << "Parser: top level" << std::endl;
        
        switch (this->peek().type()) {
            case Token::Type::KeywordDef:
                return this->parseDefinition();
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

    Scope* Parser::currentScope() const {
        assert(_currentScope);

        return _currentScope;
    }

    void Parser::pushScope(Scope* scope) {
        assert(scope);

        this->currentScope()->setParent(scope);
        _currentScope = scope;
    }

    void Parser::popScope() {
        Scope* old = this->currentScope();

        _currentScope = old->parent();
        assert(_currentScope);

        delete old;
    }
}
