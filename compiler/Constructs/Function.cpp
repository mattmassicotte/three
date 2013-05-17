#include "Function.h"

#include <sstream>

namespace Language {
    Function::~Function() {
        for (Variable* v : _parameters) {
            delete v;
        }
    }

    std::string Function::str() const {
        std::stringstream s;

        s << this->name();

        s << "(";

        for (Variable* var : _parameters) {
            s << var->dataType().str();
            s << ",";
        }

        s << ")";

        s << "return: " << this->returnType().str();

        return s.str();
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

    uint32_t Function::parameterCount() const {
        return _parameters.size();
    }

    void Function::addParameter(Variable* var) {
        _parameters.push_back(var);
    }

    void Function::addParameter(const std::string& name, const DataType& type) {
        this->addParameter(new Variable(name, type));
    }

    Variable* Function::parameterAtIndex(uint32_t index) {
        return _parameters.at(index);
    }

    void Function::eachParameter(std::function<void (Variable*, uint32_t)> func) {
        for (uint32_t i = 0; i < _parameters.size(); ++i) {
            func(_parameters[i], i);
        }
    }
}
