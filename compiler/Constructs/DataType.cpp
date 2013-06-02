#include "DataType.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    DataType::DataType(const Flavor& type) : _type(type) {
    }

    DataType::DataType(const Flavor& type, const std::string& name) : _type(type), _name(name) {
    }

    std::string DataType::str() const {
        return this->name();
    }

    DataType::Flavor DataType::flavor() const {
        return _type;
    }

    void DataType::setFlavor(const Flavor& value) {
        _type = value;
    }

    std::string DataType::name() const {
        return _name;
    }

    void DataType::setName(const std::string& string) {
        _name = string;
    }

    std::string DataType::cSourceName() const {
        if (this->flavor() == Flavor::Closure) {
            return "three_closure_t";
        } else if (this->name() == "Int") {
            return "int";
        } else if (this->name() == "Char") {
            return "char";
        } else if (this->name() == "Void") {
            return "void";
        }

        return this->name();
    }

    uint32_t DataType::childCount() const {
        return _children.size();
    }

    void DataType::addChild(const TypeReference& value) {
        _children.push_back(value);
    }

    void DataType::eachChild(std::function<void (const TypeReference&, uint32_t)> func) const {
        uint32_t index = 0;
        for (const TypeReference& child : _children) {
            func(child, index);
            ++index;
        }
    }

    bool DataType::isCallable() const {
        return _type == Flavor::Function || _type == Flavor::Closure;
    }

    TypeReference DataType::returnType() const {
        assert(this->isCallable());

        return _returnType;
    }

    void DataType::setReturnType(const TypeReference& value) {
        assert(this->isCallable());
        assert(value.referencedType());

        _returnType = value;
    }

    void DataType::eachParameterWithLast(std::function<void (const TypeReference&, bool)> func) {
        assert(this->isCallable());
        uint32_t lastIndex = this->childCount() - 1;

        this->eachChild([=] (const TypeReference& child, uint32_t index) {
            func(child, index == lastIndex);
        });
    }
}
