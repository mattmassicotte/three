#pragma once

#include "DataType.h"

namespace Language {
    class Variable {
    public:
        void setName(const std::string& value);
        std::string name() const;
        void setDataType(const DataType& type);
        DataType dataType() const;

    private:
        std::string _name;
        DataType    _type;
    };
}
