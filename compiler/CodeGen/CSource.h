#pragma once

#include <string>
#include <sstream>
#include <set>

namespace Three {
    class CSource {
    public:
        CSource();

        void print(const char* string);
        void print(const std::string& string);

        CSource& operator <<(const char* string);
        CSource& operator <<(const std::string& string);

        void printNewLine();

        void printPreviousLine(const std::string& string);
        void printLine(const std::string& string);
        void printLineAndIndent(const std::string& string);
        void outdentAndPrintLine(const std::string& string);

        void increaseIndentation();
        void decreaseIndentation();

        void addHeader(bool relative, const std::string& header);

        std::string renderToString();
        std::string renderToStringWithIncludeGuard(const std::string& name);

    protected:
        void flushLineBuffer();

    private:
        std::set<std::string> _relativeHeaders;
        std::set<std::string> _headers;

        std::stringstream _stream;
        std::stringstream _lineStream;
        uint32_t          _indentationLevel;
    };
}
