#pragma once

#include "Lexer.h"
#include "AST/RootNode.h"
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

        RootNode* rootASTNode();

        ASTNode* parseStatement();
        ASTNode* parsePrimary();
        ASTNode* parseExpression();
        bool     parseNewline(bool multipleAllowed=false);
        bool     parseUntilEnd(std::function<void (void)> func);

        TypeReference parseType();
        TypeReference parseFunctionType(uint32_t depth, std::vector<std::string>* params, std::vector<std::string>* references);

        Module*  moduleWithIdentifier(const std::string& name);
        Module*  currentModule() const;

    protected:
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
