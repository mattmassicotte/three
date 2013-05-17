#pragma once

#include "Lexer.h"
#include "AST/ASTNode.h"
#include "Constructs/Scope.h"
#include "Constructs/Module.h"

#include <string>
#include <map>

namespace Language {
    class Parser {
    public:
        Parser(Lexer* lexer);
        virtual ~Parser();

        ASTNode* rootASTNode();

        ASTNode* parseStatement();
        ASTNode* parseExpression();

        Module*  moduleWithIdentifier(const std::string& name);
        Module*  currentModule() const;

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

        std::map<std::string, Module*> _module;
        Module* _rootModule;
    };
}
