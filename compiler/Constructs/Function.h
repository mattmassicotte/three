#pragma once

#include "DataType.h"
#include "Variable.h"
#include "../Helpers/TreeStructure.hpp"

#include <vector>
#include <functional>

namespace Language {
    class Function : public TreeStructure<Variable*> {
    public:
        virtual ~Function();

        virtual std::string str() const;

        void setName(const std::string& value);
        std::string name() const;
        void setReturnType(DataType* type);
        DataType* returnType() const;

        void addParameter(const std::string& name, DataType* type);
        uint32_t parameterCount() { return this->childCount(); }
        Variable* parameterAtIndex(uint32_t index) const;


    private:
        std::string _name;
        DataType*   _returnType;
    };
}
