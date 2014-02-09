#include "Variable.h"
#include "DataType.h"

namespace Three {
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

    void Variable::codeGenCSourceForDefinition(CSource* source) {
        source->print(this->type().codeGen(this->name()));
    }
}
