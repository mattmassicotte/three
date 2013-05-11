#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace Language {
    class CSourceContext {
    public:
        typedef enum {
            NoChange,
            Indent,
            Outdent
        } Indentation;
        
    public:
        CSourceContext();

        void print(const char* string);
        void print(const std::string& string);
        void printNewLine(Indentation indent = NoChange);

        std::string renderToString();

    protected:
        void printIndent();

    private:
        std::stringstream        _stream;
        std::vector<std::string> _includes;
        uint32_t                 _indentationLevel;
    };
}
