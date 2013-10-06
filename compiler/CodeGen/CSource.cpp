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

        return _stream.str();
    }
}
