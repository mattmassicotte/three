#pragma once

#include "QualifiedName.h"

#include <string>
#include <vector>
#include <map>

namespace Three {
    class NewVariable;
    class NewDataType;
}

namespace Three {
    class NewScope {
    public:
        NewScope();
        virtual ~NewScope();

    private:
        NewScope(const NewScope& other);
        NewScope(NewScope&& other);
        NewScope& operator=(NewScope other);
        NewScope& operator=(NewScope&& other);

    public:
        bool capturing() const;
        bool capturingOrParentCapturing() const;
        void setCapturing(bool value);
        std::string scopedBasename() const;
        void setScopedBasename(const std::string& value);
        void setCurrentFunctionReturnType(const NewDataType& type);
        NewDataType currentFunctionReturnType() const;
        NewScope* parent() const;
        void setParent(NewScope* p);

        std::string scopedName(const std::string& basename);
        std::string currentScopedName(const std::string& basename);

        NewVariable* variableForName(const QualifiedName& name) const;
        bool defineVariable(NewVariable* variable);
        bool defineVariableTypeForName(const NewDataType& type, const std::string& name);

        bool referencedVariable(const std::string& name, bool passedOverCapturing=false) const;
        void addReferencedVariable(const std::string& name);
        bool capturedVariable(const std::string& name, bool passedOverCapturing=false) const;
        void captureVariable(const std::string& name);
        std::vector<std::string> capturedVariables() const;

        QualifiedName fullNamespace() const;
        void setNamespace(const QualifiedName& name);
        QualifiedName qualifiedNameWithIdentifier(const std::string& name);

    private:
        NewVariable* variableForExactName(const std::string& name) const;

    private:
        NewScope* _parent;
        bool _capturing;
        NewDataType _returnType;

        QualifiedName _namespace;

        std::map<std::string, uint32_t> _scopedNames;
        std::string _scopedBasename;

        std::map<std::string, NewVariable*> _variables;

        std::vector<std::string> _referencedVariables;
        std::vector<std::string> _capturedVariables;
    };
}
