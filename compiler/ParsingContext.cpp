#include "ParsingContext.h"

#include <assert.h>

namespace Three {
    ParsingContext* ParsingContext::translationUnitContext() {
        ParsingContext* context = new ParsingContext();

        context->_currentScope = Scope::createRootScope();
        context->_translationUnit = TranslationUnit::createTranslationUnit();

        return context;
    }

    TypeReference ParsingContext::refForName(const std::string& name, int indirectionDepth) {
        return TypeReference::ref(this->currentModule(), name, indirectionDepth);
    }

    Function* ParsingContext::functionForName(const std::string& name) {
        return NULL;
    }

    void ParsingContext::addFunction(Function* func) {
    }

    TranslationUnit* ParsingContext::translationUnit() const {
        return _translationUnit;
    }

    Module* ParsingContext::currentModule() const {
        if (_module) {
            return _module;
        }

        return _translationUnit->module();
    }

    void ParsingContext::pushModule(Module* module) {
        assert(!_module && "Only single-level modules are supported");

        module->setParent(this->currentModule());
        _module = module;
    }

    void ParsingContext::popModule() {
        assert(_module && "popModule called without anything left in the module stack");

        _module = NULL;
    }

    Scope* ParsingContext::currentScope() const {
        assert(_currentScope);

        return _currentScope;
    }

    void ParsingContext::pushScope(Scope* scope) {
        assert(scope);

        scope->setParent(this->currentScope());
        _currentScope = scope;
    }

    void ParsingContext::popScope() {
        Scope* old = this->currentScope();

        _currentScope = old->parent();
        assert(_currentScope);

        delete old;
    }
}