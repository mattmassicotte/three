#pragma once

#include <string>
#include <sstream>
#include <set>
#include <vector>

namespace Three {
    class CSource {
    public:
        static std::string includeGuard(const std::string& name);

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

        void insertLine(const std::string& str);
        uint32_t linePosition() const;
        void setLinePosition(uint32_t pos);
        void setLinePositionToEnd();

        std::string renderToString();

    protected:
        void flushLineBuffer();

    private:
        std::set<std::string> _relativeHeaders;
        std::set<std::string> _headers;

        std::vector<std::string> _lines;
        uint32_t _linePosition;

        std::stringstream _lineStream;
        uint32_t          _indentationLevel;
    };
}
