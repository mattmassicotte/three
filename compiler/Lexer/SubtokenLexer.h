#pragma once

#include "Subtoken.h"

#include <iostream>

namespace Three {
    class SubtokenLexer {
    public:
        SubtokenLexer(std::istream* stream);

    protected:
        // working with the input stream
        bool characterPeek(char& c);
        char characterAdvance();

        void populateSubtokenBuffer();

    public:
        // working with subtoken stream
        Subtoken next();
        Subtoken peek();

    protected:
        Subtoken advance();
        Subtoken lexDirective();

    private:
        bool _potentialDirective;
        std::istream* _stream;
        Subtoken      _subtoken;
    };
}
