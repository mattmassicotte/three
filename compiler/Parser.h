#pragma once

#include "Lexer.h"
#include "AST/ASTNode.h"
#include "Constructs/Scope.h"

namespace Language {
    class Parser {
    public:
        Parser(Lexer* lexer);
        virtual ~Parser();

        ASTNode* rootASTNode();

        ASTNode* parseStatement();
        ASTNode* parseExpression();

    protected:
        ASTNode* parseTopLevelNode();
        ASTNode* parseDefinition();

    public:
        Token peek(unsigned int distance=1);
        Token next();

        Scope* currentScope() const;
        void   pushScope(Scope* scope);
        void   popScope();

    private:
        Lexer* _lexer;
        Scope* _currentScope;
    };
}
