#pragma once

#include "Token.h"

#include <iostream>
#include <vector>

namespace Language {
    class Lexer {
    public:
        Lexer(std::istream* stream);

    protected:
        // working with the input stream
        bool characterPeek(char& c);
        char characterAdvance();

        void translateKeyword(Token& token);

        // working with subtoken stream
        Token nextSubtoken();
        Token peekSubtoken();
        Token advanceSubtoken();

        // lexing functions for more complex tokens
        Token lexNumericLiteral();
        Token lexAnnotation();
        Token lexForwardSlash();
        Token lexPunctuation();
        Token lexIdentifier();

        Token nextToken();

    public:
        // actual function for consuming tokens
        Token next();
        Token peek(unsigned int distance = 1);

    private:
        std::istream*      _stream;
        Token              _subtoken;
        std::vector<Token> _tokenBuffer;
    };
}