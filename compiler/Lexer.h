#pragma once

#include "Token.h"

#include <iostream>

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

    public:
        // actual function for consuming tokens
        Token nextToken();

    private:
        std::istream* _stream;
        Token         _subtoken;
    };
}