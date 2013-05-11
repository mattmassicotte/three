#include "CSourceContext.h"

#include <assert.h>
#include <iostream>

namespace Language {
    CSourceContext::CSourceContext() : _indentationLevel(0) {
    }

    void CSourceContext::print(const char* string) {
        _stream << string;
    }

    void CSourceContext::print(const std::string& string) {
        _stream << string;
    }

    void CSourceContext::printNewLine(Indentation indent) {
        _stream << std::endl;

        switch (indent) {
            case Indent:
                _indentationLevel++;
                break;
            case Outdent:
                assert(_indentationLevel > 0);
                _indentationLevel--;
                break;
            default:
                break;
        }

        this->printIndent();
    }

    void CSourceContext::printIndent() {
        _stream << std::string(_indentationLevel*4, ' ');
    }

    std::string CSourceContext::renderToString() {
        std::stringstream s;

        s << "// Rendered C code" << std::endl;
        s << std::endl;

        for (std::string path : _includes) {
            s << "#include <" << path << ">" << std::endl;
        }

        if (_includes.size() > 0) {
            s << std::endl;
        }

        s << _stream.str();

        return s.str();
    }
}
