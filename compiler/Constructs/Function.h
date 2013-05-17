#pragma once

#include "DataType.h"
#include "Variable.h"

#include <vector>

namespace Language {
    class Function {
    public:
        virtual ~Function();

        std::string str() const;

        void setName(const std::string& value);
        std::string name() const;
        void setReturnType(const DataType& type);
        DataType returnType() const;

        uint32_t parameterCount() const;
        void addParameter(Variable* var);
        void addParameter(const std::string& name, const DataType& type);
        Variable* parameterAtIndex(uint32_t index);

        void eachParameter(std::function<void (Variable*, uint32_t)> func);

    private:
        std::string            _name;
        DataType               _returnType;
        std::vector<Variable*> _parameters;
    };
}
