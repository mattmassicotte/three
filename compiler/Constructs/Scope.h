#pragma once

#include "Function.h"
#include "Variable.h"
#include "DataStructure.h"

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

        void addVariable(const std::string& name, Variable* var);
        void addFunction(const std::string& name, Function* func);
        void addDataStructure(const std::string& name, DataStructure* type);

        Variable* variableForName(const std::string& name);
        Function* functionForName(const std::string& name);
        DataStructure* dataStructureForName(const std::string& name);

    private:
        std::map<std::string, Variable*>      _variables;
        std::map<std::string, Function*>      _functions;
        std::map<std::string, DataStructure*> _dataStructures;

        Scope* _parent;
    };
}
