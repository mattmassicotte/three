#include "Variable.h"

namespace Language {
    void Variable::setName(const std::string& value) {
        _name = value;
    }

    std::string Variable::name() const {
        return _name;
    }

    void Variable::setDataType(const DataType& type) {
        _type = type;
    }

    DataType Variable::dataType() const {
        return _type;
    }
}
