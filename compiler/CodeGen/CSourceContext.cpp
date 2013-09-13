#include "CSourceContext.h"

#include <assert.h>
#include <iostream>

namespace Language {
    CSourceContext::CSourceContext() {
        _declarationSource = new CSource();
        _bodySource = new CSource();

        _currentSource = _bodySource;

        this->addHeader("three/runtime/closure.h");
    }

    CSourceContext::~CSourceContext() {
        delete _declarationSource;
        delete _bodySource;
    }

    void CSourceContext::addHeader(const std::string& header) {
        _headers.insert(header);
    }

    CSource* CSourceContext::declarations() const {
        return _declarationSource;
    }

    CSource* CSourceContext::body() const {
        return _bodySource;
    }

    CSource* CSourceContext::current() const {
        return _currentSource;
    }

    void CSourceContext::setCurrent(Section section) {
        switch (section) {
            case Section::Declarations:
                _currentSource = _declarationSource;
                break;
            case Section::Body:
                _currentSource = _bodySource;
                break;
            default:
                assert(0 && "Invalid Section");
                break;
        }
    }

    CSourceContext& CSourceContext::operator <<(const std::string& string) {
        this->current()->print(string);

        return *this;
    }

    std::string CSourceContext::renderToString() {
        std::stringstream s;

        s << "// Rendered C code" << std::endl;
        s << std::endl;

        s << "// Includes" << std::endl;
        for (const std::string& header : _headers) {
            s << "#include <" << header << ">" << std::endl;
        }
        s << std::endl;

        s << "// Declarations" << std::endl;
        s << _declarationSource->renderToString();
        s << std::endl;

        s << "// Definitions" << std::endl;
        s << _bodySource->renderToString();

        return s.str();
    }
}
