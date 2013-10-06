#pragma once

#include "Lexer.h"
#include "ParsingContext.h"

#include <string>
#include <map>

namespace Three {
    class Function;
    class TranslationUnit;
    class Scope;
    class RootNode;
    class TypeReference;
}

namespace Three {
    class Parser {
    public:
        static ParsingContext* contextFromFile(const std::string& path);

    public:
        Parser(Lexer* lexer);
        virtual ~Parser();

        RootNode* rootASTNode();

        bool parse(ParsingContext* context);

        ASTNode* parseStatement();
        ASTNode* parsePrimaryExpression();
        ASTNode* parseSecondaryExpression();
        ASTNode* parseExpression(uint32_t precedence=Token::MinimumPrecedence);
        ASTNode* parseSecondaryIdentifier();
        bool     parseNewline(bool multipleAllowed=true);
        bool     parseUntil(bool advanceOnStop, std::function<bool (const Token& token)> func);
        bool     parseUntilEnd(std::function<void (void)> func);
        std::string parseQualifiedIdentifier();

        bool isAtType();
        TypeReference parseType();
        TypeReference parseFunctionType(uint32_t depth, std::vector<std::string>* params, std::vector<std::string>* references);
        Function*     parseFunctionSignature();
        ASTNode*      parseTopLevelNode();

    public:
        Token peek(unsigned int distance=1);
        void  peekUntil(unsigned int* distance, std::function<bool (const Token& token)> func);
        Token next();
        bool  nextIf(const std::string& value);

        ParsingContext* context() const;

        TranslationUnit* currentTranslationUnit() const;
        Module* currentModule() const;
        Scope* currentScope() const;
        void   pushScope(Scope* scope);
        void   popScope();

    private:
        Lexer* _lexer;
        ParsingContext* _context;
    };
}
