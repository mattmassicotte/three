#pragma once

#include "Lexer.h"
#include "AST/ASTNode.h"

namespace Language {
    class Parser {
    public:
        Parser(Lexer* lexer);

        ASTNode rootASTNode();

    protected:
        ASTNode* parseTopLevelNode();
        ASTNode* parseDefinition();

    protected:
        Token peekToken();
        Token nextToken();

    private:
        Lexer* _lexer;
        Token  _token;
    };
}
