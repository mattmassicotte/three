#include "CSourceContext.h"

#include <assert.h>
#include <iostream>

namespace Language {
    CSourceContext::CSourceContext() : _indentationLevel(0) {
        _includes.push_back("stdio.h");
    }

    void CSourceContext::print(const char* string) {
        _stream << string;
    }

    void CSourceContext::print(const std::string& string) {
        _stream << string;
    }

    void CSourceContext::printNewLine() {
        _stream << std::endl;
        this->printIndent();
    }

    void CSourceContext::increaseIndentation() {
        ++_indentationLevel;
    }
    
    void CSourceContext::decreaseIndentation() {
        assert(_indentationLevel > 0);
        --_indentationLevel;
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
