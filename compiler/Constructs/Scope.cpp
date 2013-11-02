#include "Scope.h"

#include <assert.h>
#include <sstream>
#include <iostream>

namespace Three {
    Scope* Scope::createRootScope() {
        Scope* scope = new Scope("top_level_scope");

        return scope;
    }

    Scope::Scope(const std::string& name) : _scopedName(name), _scopeNameCount(1) {
        _transactional = false;
        _closesVariables = false;
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

    void Scope::setScopedName(const std::string& name) {
        _scopedName = name;
    }

    std::string Scope::makeScopedName(const std::string& string) {
        std::stringstream s;

        s << _scopedName << "_" << string << "_" << _scopeNameCount;
        _scopeNameCount++;

        return s.str();
    }

    void Scope::setClosingScope(bool closed) {
        _closesVariables = closed;
    }

    void Scope::setTransactional(bool value) {
        _transactional = value;
    }

    bool Scope::transactional() const {
        return _transactional;
    }

    void Scope::setNamespace(const std::string& ns) {
        _namespace = ns;
    }

    std::string Scope::namespacePrefix() const {
        return _namespace;
    }

    std::string Scope::fullNamespace() const {
        std::string ns;

        if (this->parent()) {
            ns = this->parent()->fullNamespace();

            if (ns.length() == 0) {
                return _namespace;
            }

            if (_namespace.length() == 0) {
                return ns;
            }

            return ns + "_3_" + _namespace;
        }

        return _namespace;
    }

    void Scope::addVariable(const std::string& name, Variable* var) {
        assert(var != NULL);
        _variables[name] = var;
    }

    Variable* Scope::variableForName(const std::string& name) {
        Variable* v = _variables[name];

        // if we found it, we're done
        if (v) {
            return v;
        }

        // Variable not defined in this scope.  Check the parent recursively.
        if (this->parent()) {
            return this->parent()->variableForName(name);
        }

        std::cout << "Scope: asked for a variable that does not exist '" << name << "'" << std::endl;

        return NULL;
    }

    void Scope::setReferencedVariable(const std::string& name) {
        _closureReferences[name] = this->variableForName(name);
    }

    bool Scope::referencedVariable(const std::string& name) {
        return _closureReferences[name] != NULL;
    }

    bool Scope::closedVariable(const std::string& name) {
        if (!_closesVariables) {
            return false;
        }

        if (this->_variables[name] != NULL) {
            return false;
        }

        if (this->variableForName(name) == NULL) {
            return false;
        }

        Variable* v = this->variableForName(name);

        assert(v);

        // this variable is closed, but make sure to only put it in once
        if (std::find(_closedVariables.cbegin(), _closedVariables.cend(), v) != _closedVariables.cend()) {
            return true;
        }

        _closedVariables.push_back(v);

        return true;
    }

    std::vector<Annotation*> Scope::annotations() const {
        return _annotations;
    }

    void Scope::addAnnotation(Annotation* annotation) {
        _annotations.push_back(annotation);
    }

    void Scope::clearAnnotations() {
        _annotations.clear();
    }
}
