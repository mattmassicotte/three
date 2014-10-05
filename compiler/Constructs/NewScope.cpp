#include "NewScope.h"
#include "NewVariable.h"
#include "QualifiedName.h"

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

    void NewScope::setCurrentFunctionReturnType(const NewDataType& type) {
        _returnType = type;
    }

    NewDataType NewScope::currentFunctionReturnType() const {
        if (_returnType.defined()) {
            return _returnType;
        }

        assert(_parent);

        return _parent->currentFunctionReturnType();
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

    NewVariable* NewScope::variableForExactName(const std::string& name) const {
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

    NewVariable* NewScope::variableForName(const QualifiedName& name) const {
        std::cout << "starting " << name.to_s() << " (" << this->fullNamespace().to_s() << ")" << std::endl;

        // first, search for given name
        NewVariable* v = this->variableForExactName(name.to_s());
        if (v) {
            return v;
        }

        // ok, not found. Try applying this scope's namespace
        if (_namespace.numberOfComponents() == 0) {
            return nullptr;
        }

        QualifiedName namespacedName(name);

        namespacedName.prependName(_namespace);

        return this->variableForExactName(namespacedName.to_s());
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

    QualifiedName NewScope::fullNamespace() const {
        QualifiedName name(_namespace);

        NewScope* scope = this->parent();
        while (scope) {
            name.prependName(scope->_namespace);

            scope = scope->parent();
        }

        return name;
    }

    void NewScope::setNamespace(const QualifiedName& name) {
        _namespace = name;
    }

    QualifiedName NewScope::qualifiedNameWithIdentifier(const std::string& name) {
        QualifiedName fullName(name);

        fullName.prependName(this->fullNamespace());

        return fullName;
    }
}
