#pragma once

#include <string>
#include <sstream>

namespace Language {
    class CSource {
    public:
        CSource();

        void print(const char* string);
        void print(const std::string& string);
        void printNewLine();

        void printPreviousLine(const std::string& string);
        void printLine(const std::string& string);
        void printLineAndIndent(const std::string& string);
        void outdentAndPrintLine(const std::string& string);

        void increaseIndentation();
        void decreaseIndentation();

        std::string renderToString();

    protected:
        void flushLineBuffer();

    private:
        std::stringstream _stream;
        std::stringstream _lineStream;
        uint32_t          _indentationLevel;
    };
}
