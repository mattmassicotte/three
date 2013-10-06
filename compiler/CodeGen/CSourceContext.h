#pragma once

#include "../Constructs/TranslationUnit.h"

#include <functional>
#include <string>

namespace Three {
    class CSource;
}

namespace Three {
    class CSourceContext {
    public:
        CSourceContext();
        virtual ~CSourceContext();

        // sections
        CSource* externalDeclarations() const;
        CSource* internalDeclarations() const;
        CSource* declarations() const;

        CSource* body() const;

        CSource* current() const;
        void setCurrent(CSource* source);
        CSource* declaractionSourceForVisibility(TranslationUnit::Visibility v) const;

        void adjustCurrent(CSource* source, std::function<void (CSource*)> func);
        void adjustCurrentForVisibility(TranslationUnit::Visibility v, std::function<void (CSource*)> func);

        CSourceContext& operator <<(const char* string);
        CSourceContext& operator <<(const std::string& string);

    private:
        CSource* _externalDeclarationSource;
        CSource* _internalDeclarationSource;
        CSource* _declarationSource;
        CSource* _bodySource;
        CSource* _currentSource;
    };
}
