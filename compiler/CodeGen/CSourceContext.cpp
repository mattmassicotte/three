#include "CSourceContext.h"
#include "CSource.h"

#include <assert.h>

namespace Three {
    CSourceContext::CSourceContext() {
        _externalDeclarationSource = new CSource();
        _internalDeclarationSource = new CSource();
        _declarationSource = new CSource();
        _bodySource = new CSource();

        _declarationSource->addHeader(false, "three/runtime/types.h");

        this->setCurrent(_bodySource);
    }

    CSourceContext::~CSourceContext() {
        delete _externalDeclarationSource;
        delete _internalDeclarationSource;
        delete _declarationSource;
        delete _bodySource;
    }

    // sections
    CSource* CSourceContext::externalDeclarations() const {
        return _externalDeclarationSource;
    }

    CSource* CSourceContext::internalDeclarations() const {
        return _internalDeclarationSource;
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

    void CSourceContext::setCurrent(CSource* source) {
        assert(source == _bodySource ||
               source == _externalDeclarationSource ||
               source == _internalDeclarationSource ||
               source == _declarationSource);

        _currentSource = source;
    }

    CSource* CSourceContext::declaractionSourceForVisibility(TranslationUnit::Visibility v) const {
        switch (v) {
            case TranslationUnit::Visibility::None:
                return this->declarations();
            case TranslationUnit::Visibility::Internal:
            case TranslationUnit::Visibility::Default:
                return this->internalDeclarations();
            case TranslationUnit::Visibility::External:
                return this->externalDeclarations();
            default:
                break;
        }

        assert(0 && "Unrecognized visibility type");
    }

    void CSourceContext::adjustCurrent(CSource* source, std::function<void (CSource*)> func) {
        CSource* original = this->current();

        this->setCurrent(source);

        func(this->current());

        this->setCurrent(original);
    }

    void CSourceContext::adjustCurrentForVisibility(TranslationUnit::Visibility v, std::function<void (CSource*)> func) {
        this->adjustCurrent(this->declaractionSourceForVisibility(v), func);
    }

    CSourceContext& CSourceContext::operator <<(const char* string) {
        *_currentSource << string;

        return *this;
    }

    CSourceContext& CSourceContext::operator <<(const std::string& string) {
        *_currentSource << string;

        return *this;
    }
}
