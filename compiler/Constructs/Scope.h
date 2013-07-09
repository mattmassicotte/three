#pragma once

#include "Annotation.h"
#include "Variable.h"

#include <string>
#include <map>
#include <vector>

namespace Language {
    class Scope {
    public:
        static Scope* createRootScope();

    public:
        Scope(const std::string& name);
        virtual ~Scope();

        void setParent(Scope* value);
        Scope* parent() const;

        void setScopedName(const std::string& name);
        std::string makeScopedName(const std::string& string);
        void setClosingScope(bool closed);

        Variable* variableForName(const std::string& name);
        void addVariable(const std::string& name, Variable* var);
        void setReferencedVariable(const std::string& name);
        bool referencedVariable(const std::string& name);
        bool closedVariable(const std::string& name);

        std::vector<Annotation*> annotations() const;
        void addAnnotation(Annotation* annotation);
        void clearAnnotations();

        std::vector<Variable*> _closedVariables; // TODO: this is not right

    private:
        std::map<std::string, Variable*> _variables;
        std::map<std::string, Variable*> _closureReferences;
        std::vector<Annotation*>         _annotations;

        bool        _closesVariables;
        std::string _scopedName;
        uint32_t    _scopeNameCount;

        Scope* _parent;
    };
}
