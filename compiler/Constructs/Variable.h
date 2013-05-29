#pragma once

#include "DataType.h"

namespace Language {
    class Variable {
    public:
        Variable();
        Variable(const std::string& name, DataType* type);

        void setName(const std::string& value);
        std::string name() const;
        void setType(DataType* type);
        DataType* type() const;

    private:
        std::string _name;
        DataType*   _type;
    };
}
