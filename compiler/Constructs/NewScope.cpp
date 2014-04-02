#include "NewScope.h"
#include "NewVariable.h"

#include <sstream>

namespace Three {
    NewScope::NewScope() : _capturing(false), _parent(nullptr) {
    }

    NewScope::~NewScope() {
    }

    bool NewScope::capturing() const {
        return _capturing;
    }

    bool NewScope::capturingOrParentCapturing() const {
        if (_capturing) {
            return true;
        }

        // this is an inherented property
        if (_parent) {
            return _parent->capturing();
        }

        return false;

    }

    void NewScope::setCapturing(bool value) {
        _capturing = value;
    }

    std::string NewScope::scopedBasename() const {
        return _scopedBasename;
    }

    void NewScope::setScopedBasename(const std::string& value) {
        _scopedBasename = value;
    }

    NewScope* NewScope::parent() const {
        return _parent;
    }

    void NewScope::setParent(NewScope* p) {
        _parent = p;
    }

    std::string NewScope::scopedName(const std::string& name) {
        if (_scopedBasename.length() == 0) {
            assert(_parent);
            return _parent->scopedName(name);
        }

        uint32_t value = _scopedNames[name];

        value += 1;

        _scopedNames[name] = value;

        std::stringstream s;

        s << _scopedBasename << "_" << name << "_" << value;

        return s.str();
    }

    std::string NewScope::currentScopedName(const std::string& basename) {
        auto it = _scopedNames.find(basename);

        if (it == _scopedNames.cend()) {
            return "";
        }

        std::stringstream s;

        s << _scopedBasename << "_" << basename << "_" << it->second;

        return s.str();
    }

    NewVariable* NewScope::variableForName(const std::string& name) const {
        auto it = _variables.find(name);

        if (it != _variables.cend()) {
            return it->second;
        }

        // search through parents
        if (!_parent) {
            return nullptr;
        }

        return _parent->variableForName(name);
    }

    bool NewScope::defineVariable(NewVariable* variable) {
        _variables[variable->name] = variable;

        return true;
    }

    bool NewScope::defineVariableTypeForName(const NewDataType& type, const std::string& name) {
        NewVariable* variable = new NewVariable();

        variable->name = name;
        variable->type = type;

        return this->defineVariable(variable);
    }

    bool NewScope::referencedVariable(const std::string& name, bool passedOverCapturing) const {
        if (std::find(_referencedVariables.cbegin(), _referencedVariables.cend(), name) != _referencedVariables.cend()) {
            return true;
        }

        if (!_parent) {
            return false;
        }

        // Once we encounter a scope that captures, we need to stop checking
        // for referenced variables. This is to deal with nested closures. This
        // is doubly-tricky because of the recursion.
        if (passedOverCapturing) {
            return false;
        }

        passedOverCapturing = passedOverCapturing || _parent->capturing();

        // special-case, closure directly nested in another closure
        if (this->capturing() && passedOverCapturing) {
            return false;
        }

        return _parent->referencedVariable(name, passedOverCapturing);
    }

    void NewScope::addReferencedVariable(const std::string& name) {
        assert(!this->referencedVariable(name));

        _referencedVariables.push_back(name);
    }

    bool NewScope::capturedVariable(const std::string& name, bool passedOverCapturing) const {
        // if this scope defined the variable, it is only captured if we've passed over
        // a closure
        if (_variables.find(name) != _variables.cend()) {
            return passedOverCapturing;
        }

        // if we have no parent, we haven't found the variable at all
        if (!_parent) {
            return false;
        }

        // if we are currently capturing, then we've now passed over a capturing scope
        passedOverCapturing = passedOverCapturing || this->capturing();

        return _parent->capturedVariable(name, passedOverCapturing);
    }

    void NewScope::captureVariable(const std::string& name) {
        if (std::find(_capturedVariables.cbegin(), _capturedVariables.cend(), name) != _capturedVariables.cend()) {
            return;
        }

        _capturedVariables.push_back(name);
    }

    std::vector<std::string> NewScope::capturedVariables() const {
        return _capturedVariables;
    }

    std::vector<std::string> NewScope::fullNamespace() const {
        std::vector<std::string> components;

        const NewScope* scope = this;
        while (scope) {
            if (scope->namespaceString.size() > 0) {
                components.insert(components.cbegin(), scope->namespaceString);
            }

            scope = scope->parent();
        }

        return components;
    }
}
