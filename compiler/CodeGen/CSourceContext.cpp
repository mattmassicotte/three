#include "CSourceContext.h"

#include <assert.h>
#include <iostream>

namespace Language {
    CSourceContext::CSourceContext() {
        _headerSource = new CSource();
        _declarationSource = new CSource();
        _bodySource = new CSource();

        _currentSource = _bodySource;
    }

    CSourceContext::~CSourceContext() {
        delete _headerSource;
        delete _declarationSource;
        delete _bodySource;
    }

    CSource* CSourceContext::headers() const {
        return _headerSource;
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
        s << "#include <stdbool.h>" << std::endl;
        s << _headerSource->renderToString();
        s << std::endl;

        s << "// Closure Support" << std::endl;
        s << "typedef void* (*three_closure_function_t)(void*, ...);" << std::endl;
        s << "typedef struct {" << std::endl;
        s << "    three_closure_function_t function;" << std::endl;
        s << "    int   env_size;" << std::endl;
        s << "    void* env;" << std::endl;
        s << "} three_closure_t;" << std::endl;
        s << "#define THREE_MAKE_CLOSURE(func) ((three_closure_t){(three_closure_function_t)func, sizeof(struct func ## _env), (void*)&(func ## _env ## _value)})" << std::endl;
        s << "#define THREE_CALL_CLOSURE(clos_type, clos, ...) (((clos_type)clos.function)(clos.env, __VA_ARGS__))" << std::endl;
        s << std::endl;

        s << "// Declarations" << std::endl;
        s << _declarationSource->renderToString();
        s << std::endl;

        s << "// Definitions" << std::endl;
        s << _bodySource->renderToString();

        return s.str();
    }
}
