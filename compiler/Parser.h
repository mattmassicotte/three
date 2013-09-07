#pragma once

#include "Lexer.h"
#include "ParsingContext.h"
#include "AST/RootNode.h"
#include "Constructs/Scope.h"
#include "Constructs/TranslationUnit.h"
#include "Constructs/TypeReference.h"
#include "Constructs/Function.h"

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
        bool     parseNewline(bool multipleAllowed=true);
        bool     parseUntil(bool advanceOnStop, std::function<bool (const Token& token)> func);
        bool     parseUntilEnd(std::function<void (void)> func);
        std::string parseQualifiedName();

        bool isAtType();
        TypeReference parseType();
        TypeReference parseFunctionType(uint32_t depth, std::vector<std::string>* params, std::vector<std::string>* references);
        Function*     parseFunctionSignature();
        ASTNode*      parseTopLevelNode();

    public:
        Token peek(unsigned int distance=1);
        Token next();
        bool  nextIf(const std::string& value);

        Three::ParsingContext* context() const;
        void setContext(Three::ParsingContext* context);

        TranslationUnit* currentTranslationUnit() const;
        Three::Module* currentModule() const;
        Scope* currentScope() const;
        void   pushScope(Scope* scope);
        void   popScope();

    private:
        Lexer* _lexer;
        Three::ParsingContext* _context;
    };
}
