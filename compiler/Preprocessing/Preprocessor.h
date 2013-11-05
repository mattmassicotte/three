#pragma once

#include "../Lexer/SubtokenLexer.h"

#include <iostream>
#include <sstream>

namespace Three {
    class Preprocessor {
    public:
        static std::string process(std::istream* stream);

    public:
        Preprocessor(SubtokenLexer* lexer);

        std::string processedOutput();

        void process();

    protected:
        void parseDirective();
        void parseIf();

    private:
        SubtokenLexer* _lexer;
        std::stringstream _processedStream;
    };
}
