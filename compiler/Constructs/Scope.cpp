#include "Scope.h"

#include <assert.h>

namespace Language {
    Scope* Scope::createRootScope() {
        Scope* scope = new Scope();

        return scope;
    }

    Scope::Scope() {
        _parent = NULL;
    }

    Scope::~Scope() {
        // for (auto& kv : _variables) {
        //     delete kv.second;
        // }
        // 
        // for (auto& kv : _functions) {
        //     delete kv.second;
        // }
        // 
        // for (auto& kv : _dataStructures) {
        //     delete kv.second;
        // }
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

    Variable* Scope::variableForName(const std::string& name) {
        // TODO: cannot use the [] operator here
        return _variables[name];
    }
}
