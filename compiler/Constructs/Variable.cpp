#include "Variable.h"

namespace Language {
    Variable::Variable() {
    }

    Variable::Variable(const std::string& name, const DataType& type) : _name(name), _type(type) {
    }

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
