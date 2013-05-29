#pragma once

#include "Variable.h"

#include <string>
#include <map>

namespace Language {
    class Scope {
    public:
        static Scope* createRootScope();

    public:
        Scope();
        virtual ~Scope();

        void setParent(Scope* value);
        Scope* parent() const;

        Variable* variableForName(const std::string& name);
        void addVariable(const std::string& name, Variable* var);

    private:
        std::map<std::string, Variable*> _variables;

        Scope* _parent;
    };
}
