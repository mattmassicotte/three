#include "CSource.h"

#include <assert.h>
#include <iostream>

namespace Three {
    CSource::CSource() : _indentationLevel(0) {
    }

    void CSource::print(const char* string) {
        // std::cout << "printing: '" << string << "'" << std::endl;
        _lineStream << string;
    }

    void CSource::print(const std::string& string) {
        this->print(string.c_str());
    }

    CSource& CSource::operator <<(const char* string) {
        this->print(string);

        return *this;
    }

    CSource& CSource::operator <<(const std::string& string) {
        this->print(string);

        return *this;
    }

    void CSource::printNewLine() {
        this->printLine("");
    }

    void CSource::printPreviousLine(const std::string& string) {
        _stream << std::string(_indentationLevel*4, ' ');
        _stream << string << std::endl;
    }

    void CSource::printLine(const std::string& string) {
        // std::cout << "printing line: '" << string << "'" << std::endl;
        // std::cout << "current buffer: '" << _lineStream.str() << "'" << std::endl;

        _lineStream << string << std::endl;
        this->flushLineBuffer();
    }

    void CSource::printLineAndIndent(const std::string& string) {
        this->printLine(string);
        this->increaseIndentation();
    }

    void CSource::outdentAndPrintLine(const std::string& string) {
        this->decreaseIndentation();
        this->printLine(string);
    }

    void CSource::increaseIndentation() {
        _indentationLevel += 1;
        // std::cout << "indentation now: " << _indentationLevel << std::endl;
    }
    
    void CSource::decreaseIndentation() {
        assert(_indentationLevel > 0);
        _indentationLevel -= 1;
        // std::cout << "indentation now: " << _indentationLevel << std::endl;
    }

    void CSource::addHeader(bool relative, const std::string& header) {
        if (relative) {
            _relativeHeaders.insert(header);
        } else {
            _headers.insert(header);
        }
    }

    void CSource::flushLineBuffer() {
        if (_lineStream.tellp() == 0) {
            return;
        }

        // std::cout << "flushing line buffer '" << _lineStream.str() << "'" << std::endl;

        // commit the current line
        _stream << std::string(_indentationLevel*4, ' ');
        _stream << _lineStream.str();

        _lineStream.str(""); // reset the line buffer
        _lineStream.clear(); // reset errors (not content)
    }

    std::string CSource::renderToString() {
        this->flushLineBuffer();

        if (_headers.size() == 0 && _relativeHeaders.size() == 0) {
            return _stream.str();
        }

        std::stringstream s;

        for (const std::string& header : _headers) {
            s << "#include <" + header + ">" << std::endl;
        }

        if (_headers.size() > 0) {
            s << std::endl;
        }

        for (const std::string& header : _relativeHeaders) {
            s << "#include \"" + header + "\"" << std::endl;
        }

        if (_relativeHeaders.size() > 0) {
            s << std::endl;
        }

        s << _stream.str();

        return s.str();
    }

    std::string CSource::renderToStringWithIncludeGuard(const std::string& name) {
        std::stringstream s;

        // #pragma once is a way, way better construct.  However, due to a bug in
        // clang, you cannot suppress that warning.

        s << "#pragma once" << std::endl << std::endl;

        s << this->renderToString();

        return s.str();
    }
}
