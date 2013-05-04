#include "Parser.h"

namespace Language {
    Parser::Parser(Lexer* lexer) : _lexer(lexer) {
        _token = _lexer->nextToken();
    }

    ASTNode Parser::rootASTNode() {
        ASTNode root;

        while (!this->peekToken().isEnd()) {
            root.addChild(this->parseTopLevelNode());
        }

        return root;
    }

    ASTNode* Parser::parseTopLevelNode() {
        switch (this->peekToken().type()) {
            case Token::Type::KeywordDef:
                return this->parseDefinition();
            default:
                this->nextToken();
                return new ASTNode();
        }

        return NULL;
    }

    ASTNode* Parser::parseDefinition() {
        this->nextToken();

        return new ASTNode();
    }

    Token Parser::peekToken() {
        return _token;
    }

    Token Parser::nextToken() {
        Token tmp = _token;

        _token = _lexer->nextToken();

        return tmp;
    }
}
