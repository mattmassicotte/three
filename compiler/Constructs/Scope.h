#pragma once

#include "QualifiedName.h"

#include <string>
#include <vector>
#include <map>

namespace Three {
    class Variable;
    class DataType;
}

namespace Three {
    class Scope {
    public:
        Scope();
        virtual ~Scope();

    private:
        Scope(const Scope& other);
        Scope(Scope&& other);
        Scope& operator=(Scope other);
        Scope& operator=(Scope&& other);

    public:
        bool capturing() const;
        bool capturingOrParentCapturing() const;
        void setCapturing(bool value);
        std::string scopedBasename() const;
        void setScopedBasename(const std::string& value);
        void setCurrentFunctionReturnType(const DataType& type);
        DataType currentFunctionReturnType() const;
        Scope* parent() const;
        void setParent(Scope* p);

        std::string scopedName(const std::string& basename);
        std::string currentScopedName(const std::string& basename);

        Variable* variableForName(const QualifiedName& name) const;
        bool defineVariable(Variable* variable);
        bool defineVariableTypeForName(const DataType& type, const QualifiedName& name);

        bool referencedVariable(const std::string& name, bool passedOverCapturing=false) const;
        void addReferencedVariable(const std::string& name);
        bool capturedVariable(const std::string& name, bool passedOverCapturing=false) const;
        void captureVariable(const std::string& name);
        std::vector<std::string> capturedVariables() const;

        QualifiedName fullNamespace() const;
        void setNamespace(const QualifiedName& name);
        QualifiedName qualifiedNameWithIdentifier(const std::string& name);

    private:
        Scope* _parent;
        bool _capturing;
        DataType _returnType;

        QualifiedName _namespace;

        std::map<std::string, uint32_t> _scopedNames;
        std::string _scopedBasename;

        std::map<std::string, Variable*> _variables;

        std::vector<std::string> _referencedVariables;
        std::vector<std::string> _capturedVariables;
    };
}
