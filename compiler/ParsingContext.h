#pragma once

#include "Constructs/Function.h"
#include "Constructs/Scope.h"
#include "Constructs/TranslationUnit.h"
#include "Constructs/TypeReference.h"

#include <string>

namespace Three {
    class ParsingContext {
    public:
        static ParsingContext* translationUnitContext();

    public:
        TypeReference refForName(const std::string& name, int indirectionDepth);
        Function*     functionForName(const std::string& name);
        void          addFunction(Function* func);

        TranslationUnit* translationUnit() const;

        Module* currentModule() const;
        void    pushModule(Module* module);
        void    popModule();

        Scope* currentScope() const;
        void   pushScope(Scope* scope);
        void   popScope();

        void setVisibility(TranslationUnit::Visibility visibility);
        TranslationUnit::Visibility visibility() const;

    private:
        TranslationUnit* _translationUnit;
        Scope*           _currentScope;
        Module*          _module;
        TranslationUnit::Visibility _visibility;
    };
}
