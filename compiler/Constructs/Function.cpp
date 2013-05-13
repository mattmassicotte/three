#include "Function.h"

namespace Language {
    Function::~Function() {
        // TODO: remove all parameters
    }

    void Function::setName(const std::string& value) {
        _name = value;
    }

    std::string Function::name() const {
        return _name;
    }

    void Function::setReturnType(const DataType& type) {
        _returnType = type;
    }

    DataType Function::returnType() const {
        return _returnType;
    }
}
