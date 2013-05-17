#pragma once

#include "DataType.h"

namespace Language {
    class Variable {
    public:
        Variable();
        Variable(const std::string& name, const DataType& type);

        void setName(const std::string& value);
        std::string name() const;
        void setDataType(const DataType& type);
        DataType dataType() const;

    private:
        std::string _name;
        DataType    _type;
    };
}
