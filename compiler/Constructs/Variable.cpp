#include "Variable.h"

namespace Language {
    Variable::Variable() {
    }

    Variable::Variable(const std::string& name, const TypeReference& type) : _name(name), _type(type) {
    }

    void Variable::setName(const std::string& value) {
        _name = value;
    }

    std::string Variable::name() const {
        return _name;
    }

    void Variable::setType(const TypeReference& type) {
        _type = type;
    }

    TypeReference Variable::type() const {
        return _type;
    }
}
