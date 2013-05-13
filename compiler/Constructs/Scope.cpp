#include "Scope.h"

#include <assert.h>

namespace Language {
    Scope* Scope::createRootScope() {
        Scope* scope = new Scope();

        scope->addDataStructure("Void", new DataStructure("Void"));
        scope->addDataStructure("Int",  new DataStructure("Int"));
        scope->addDataStructure("Char", new DataStructure("Char"));

        return scope;
    }

    Scope::Scope() {
        _parent = NULL;
    }

    Scope::~Scope() {
        for (auto& kv : _variables) {
            delete kv.second;
        }

        for (auto& kv : _functions) {
            delete kv.second;
        }

        for (auto& kv : _dataStructures) {
            delete kv.second;
        }
    }

    void Scope::setParent(Scope* value) {
        assert(value);
        _parent = value;
    }

    Scope* Scope::parent() const {
        return _parent;
    }

    void Scope::addVariable(const std::string& name, Variable* var) {
        assert(var != NULL);
        _variables[name] = var;
    }

    void Scope::addFunction(const std::string& name, Function* func) {
        assert(func != NULL);
        _functions[name] = func;
    }

    void Scope::addDataStructure(const std::string& name, DataStructure* type) {
        assert(type != NULL);
        _dataStructures[name] = type;
    }

    Variable* Scope::variableForName(const std::string& name) {
        // TODO: cannot use the [] operator here
        return _variables[name];
    }

    Function* Scope::functionForName(const std::string& name) {
        // TODO: cannot use the [] operator here,
        // and also need to go up to a scope that can contain functions
        return _functions[name];
    }

    DataStructure* Scope::dataStructureForName(const std::string& name) {
        // TODO: cannot use the [] operator here,
        // and also need to go up to a scope that can contain functions
        return _dataStructures[name];
    }
}
