#pragma once

#include "compiler/Lexer/Token.h"

#include <string>

namespace Three {
    class Lexer;
}

namespace Three {
    class ParseHelper {
    public:
        ParseHelper(Lexer* lexer);
        virtual ~ParseHelper();

    private:
        ParseHelper(const ParseHelper& other);
        ParseHelper(ParseHelper&& other);
        ParseHelper& operator=(ParseHelper other);
        ParseHelper& operator=(ParseHelper&& other);

    public:
        Lexer* lexer() const;

        Token peek(unsigned int distance=1);
        Token next();
        std::string nextStr();
        bool peekCompare(const std::string& value);
        bool peekCompare(const Token::Type& type);
        bool nextIf(const std::string& value);
        bool nextIf(const Token::Type& type);
        bool peekUntil(unsigned int* distance, std::function<bool (const Token& token)> func);
        bool parseUntil(bool advanceOnStop, std::function<bool (const Token& token)> func);
        bool parseUntilEnd(std::function<bool (void)> func);
        bool parseNewline();
        bool parseNewlines();

        bool parseMatchingTokens(Token::Type opening, Token::Type closing, std::function<void (void)> func);

    private:
        Lexer* _lexer;
    };
}
