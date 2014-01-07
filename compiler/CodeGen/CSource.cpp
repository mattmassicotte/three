#include "CSource.h"
#include "runtime/platform.h"

#include <assert.h>
#include <iostream>

namespace Three {
    CSource::CSource() : _indentationLevel(0), _linePosition(0) {
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
        std::stringstream s;

        s << std::string(_indentationLevel*4, ' ');
        s << string << std::endl;

        this->insertLine(s.str());
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
        this->insertLine(std::string(_indentationLevel*4, ' ') + _lineStream.str());

        _lineStream.str(""); // reset the line buffer
        _lineStream.clear(); // reset errors (not content)
    }

    void CSource::insertLine(const std::string& str) {
        _lines.insert(_lines.begin() + _linePosition, str);
        _linePosition++;
    }

    uint32_t CSource::linePosition() const {
        return _linePosition;
    }

    void CSource::setLinePosition(uint32_t pos) {
        _linePosition = pos;
        assert(_linePosition < _lines.size());
    }

    void CSource::setLinePositionToEnd() {
        _linePosition = _lines.size() - 1;
    }

    std::string CSource::renderToString() {
        this->flushLineBuffer();

        if (_headers.size() == 0 && _relativeHeaders.size() == 0) {
            std::stringstream s;

            for (const std::string& str : _lines) {
                s << str;
            }

            return s.str();
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

        for (const std::string& str : _lines) {
            s << str;
        }

        return s.str();
    }

    std::string CSource::renderToStringWithIncludeGuard(const std::string& name) {
        std::stringstream s;

        // #pragma once is a way, way better construct.  However, due to a bug in
        // clang, you cannot suppress that warning.

        s << "#pragma once" << std::endl << std::endl;

        // The feature flags in the linux headers need to be correctly activated
        // for all C sources.  This is one easy way to do it.
#if THREE_PLATFORM_LINUX
        s << "#ifndef _GNU_SOURCE" << std::endl;
        s << "#define _GNU_SOURCE" << std::endl;
        s << "#endif" << std::endl << std::endl;
#endif

        s << this->renderToString();

        return s.str();
    }
}
