#pragma once

#include "Lexer.h"
#include "AST/ASTNode.h"
#include "Constructs/Scope.h"
#include "Constructs/Module.h"
#include "Constructs/TypeReference.h"

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

        TypeReference parseType();

        Module*  moduleWithIdentifier(const std::string& name);
        Module*  currentModule() const;

    protected:
        TypeReference parseFunctionType(uint32_t depth);
        ASTNode* parseTopLevelNode();
        ASTNode* parseDefinition();

    public:
        Token peek(unsigned int distance=1);
        Token next();
        bool  nextIf(const std::string& value);

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
