#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace Language {
    class CSourceContext {
    public:
        CSourceContext();

        void print(const char* string);
        void print(const std::string& string);
        void printNewLine();

        void increaseIndentation();
        void decreaseIndentation();

        std::string renderToString();

    protected:
        void printIndent();

    private:
        std::stringstream        _stream;
        std::vector<std::string> _includes;
        uint32_t                 _indentationLevel;
    };
}
