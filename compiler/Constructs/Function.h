#pragma once

#include "DataType.h"
#include "Variable.h"

#include <vector>

namespace Language {
    class Function {
    public:
        virtual ~Function();

        void setName(const std::string& value);
        std::string name() const;
        void setReturnType(const DataType& type);
        DataType returnType() const;

    private:
        std::string            _name;
        DataType               _returnType;
        std::vector<Variable*> _parameters;
    };
}
