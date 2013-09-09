#include "Function.h"
#include "DataType.h"

#include <assert.h>
#include <iostream>
#include <sstream>

namespace Language {
    Function::~Function() {
        // this->eachChild([=] (Variable* v, uint32_t index) {
        //     delete v;
        // });
    }

    std::string Function::str() const {
        std::stringstream s;

        s << this->name();

        s << "(";

        this->eachParameterWithLast([=, &s] (Variable* v, bool last) {
            s << v->type().referencedType()->str();
            if (!last) {
                s << ",";
            }
        });

        s << ")";

        s << "return: " << this->returnType().referencedType()->str();
        
        return s.str();
    }

    void Function::setName(const std::string& value) {
        _name = value;
    }

    std::string Function::name() const {
        return _name;
    }

    void Function::setNamespace(const std::string& ns) {
        _namespace = ns;
    }

    std::string Function::namespacePrefix() const {
        return _namespace;
    }

    void Function::setReturnType(const TypeReference& type) {
        assert(type.referencedType());
        _returnType = type;
    }

    TypeReference Function::returnType() const {
        return _returnType;
    }

    DataType* Function::createType() const {
        DataType* type = new DataType(Language::DataType::Flavor::Function, this->name());;

        type->setReturnType(this->returnType());

        this->eachParameterWithLast([&] (Variable* v, bool last) {
            type->addChild(v->type());
        });

        return type;
    }

    uint32_t Function::parameterCount() const {
        return _parameters.size();
    }

    void Function::addParameter(const std::string& name, const TypeReference& type) {
        _parameters.push_back(new Variable(name, type));
    }

    Variable* Function::parameterAtIndex(uint32_t index) const {
        return _parameters.at(index);
    }

    void Function::eachParameterWithLast(std::function<void (Variable*, bool)> func) const {
        uint32_t lastIndex = this->parameterCount() - 1;

        for (uint32_t i = 0; i < this->parameterCount(); ++i) {
            func(this->parameterAtIndex(i), lastIndex == i);
        }
    }

    void Function::codeGenCSource(CSourceContext& context, std::function<void (void)> func) const {
        this->returnType().codeGenCSource(context.current(), "");

        context << " " << this->name() << "(";

        this->eachParameterWithLast([=, &context] (Variable* var, bool last) {
            var->type().codeGenCSource(context.current(), var->name());
            if (!last) {
                context << ", ";
            }
        });
        
        if (this->parameterCount() == 0) {
            context << "void";
        }

        if (!func) {
            context.current()->printLine(");");
            return;
        }

        context.current()->printLineAndIndent(") {");

        func();

        context.current()->outdentAndPrintLine("}");
    }
}
