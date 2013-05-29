#include "Function.h"

#include <iostream>
#include <sstream>

namespace Language {
    Function::~Function() {
        this->eachChild([=] (Variable* v, uint32_t index) {
            delete v;
        });
    }

    std::string Function::str() const {
        std::stringstream s;

        s << this->name();

        s << "(";

        this->eachChild([=, &s] (Variable* v, bool last) {
            s << v->type()->str();
            if (!last) {
                s << ",";
            }
        });

        s << ")";

        s << "return: " << this->returnType()->str();
        
        return s.str();
    }

    void Function::setName(const std::string& value) {
        _name = value;
    }

    std::string Function::name() const {
        return _name;
    }

    void Function::setReturnType(DataType* type) {
        _returnType = type;
    }

    DataType* Function::returnType() const {
        return _returnType;
    }

    void Function::addParameter(const std::string& name, DataType* type) {
        this->addChild(new Variable(name, type));
    }

    Variable* Function::parameterAtIndex(uint32_t index) const {
        return this->childAtIndex(index);
    }
}
