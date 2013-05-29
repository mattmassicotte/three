#include "Variable.h"

namespace Language {
    Variable::Variable() {
    }

    Variable::Variable(const std::string& name, DataType* type) : _name(name), _type(type) {
    }

    void Variable::setName(const std::string& value) {
        _name = value;
    }

    std::string Variable::name() const {
        return _name;
    }

    void Variable::setType(DataType* type) {
        _type = type;
    }

    DataType* Variable::type() const {
        return _type;
    }
}
