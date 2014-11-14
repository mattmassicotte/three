#include "Scope.h"
#include "NewVariable.h"
#include "QualifiedName.h"

#include <sstream>

namespace Three {
    Scope::Scope() : _capturing(false), _parent(nullptr) {
    }

    Scope::~Scope() {
    }

    bool Scope::capturing() const {
        return _capturing;
    }

    bool Scope::capturingOrParentCapturing() const {
        if (_capturing) {
            return true;
        }

        // this is an inherented property
        if (_parent) {
            return _parent->capturing();
        }

        return false;

    }

    void Scope::setCapturing(bool value) {
        _capturing = value;
    }

    std::string Scope::scopedBasename() const {
        return _scopedBasename;
    }

    void Scope::setScopedBasename(const std::string& value) {
        _scopedBasename = value;
    }

    void Scope::setCurrentFunctionReturnType(const DataType& type) {
        _returnType = type;
    }

    DataType Scope::currentFunctionReturnType() const {
        if (_returnType.defined()) {
            return _returnType;
        }

        assert(_parent);

        return _parent->currentFunctionReturnType();
    }

    Scope* Scope::parent() const {
        return _parent;
    }

    void Scope::setParent(Scope* p) {
        _parent = p;
    }

    std::string Scope::scopedName(const std::string& name) {
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

    std::string Scope::currentScopedName(const std::string& basename) {
        auto it = _scopedNames.find(basename);

        if (it == _scopedNames.cend()) {
            return "";
        }

        std::stringstream s;

        s << _scopedBasename << "_" << basename << "_" << it->second;

        return s.str();
    }

    NewVariable* Scope::variableForName(const std::string& name) const {
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

    bool Scope::defineVariable(NewVariable* variable) {
        _variables[variable->name] = variable;

        return true;
    }

    bool Scope::defineVariableTypeForName(const DataType& type, const std::string& name) {
        NewVariable* variable = new NewVariable();

        variable->name = name;
        variable->type = type;

        return this->defineVariable(variable);
    }

    bool Scope::referencedVariable(const std::string& name, bool passedOverCapturing) const {
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

    void Scope::addReferencedVariable(const std::string& name) {
        assert(!this->referencedVariable(name));

        _referencedVariables.push_back(name);
    }

    bool Scope::capturedVariable(const std::string& name, bool passedOverCapturing) const {
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

    void Scope::captureVariable(const std::string& name) {
        if (std::find(_capturedVariables.cbegin(), _capturedVariables.cend(), name) != _capturedVariables.cend()) {
            return;
        }

        _capturedVariables.push_back(name);
    }

    std::vector<std::string> Scope::capturedVariables() const {
        return _capturedVariables;
    }

    QualifiedName Scope::fullNamespace() const {
        QualifiedName name(_namespace);

        Scope* scope = this->parent();
        while (scope) {
            name.prependName(scope->_namespace);

            scope = scope->parent();
        }

        return name;
    }

    void Scope::setNamespace(const QualifiedName& name) {
        _namespace = name;
    }

    QualifiedName Scope::qualifiedNameWithIdentifier(const std::string& name) {
        QualifiedName fullName(name);

        fullName.prependName(this->fullNamespace());

        return fullName;
    }
}
