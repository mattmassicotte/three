#pragma once

#include "Lexer.h"
#include "AST/ASTNode.h"

namespace Language {
    class Parser {
    public:
        Parser(Lexer* lexer);

        ASTNode* rootASTNode();

        ASTNode* parseStatement();
        ASTNode* parseExpression();

    protected:
        ASTNode* parseTopLevelNode();
        ASTNode* parseDefinition();

    public:
        Token peek(unsigned int distance=1);
        Token next();

    private:
        Lexer* _lexer;
    };
}
