#include "DataType.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    DataType::DataType(const Flavor& type) : _type(type), _children(), _prependStructKeyword(false) {
    }

    DataType::DataType(const Flavor& type, const std::string& name) : _type(type), _children(), _prependStructKeyword(false) {
        this->setName(name);
    }

    std::string DataType::str() const {
        if (!this->isCallable()) {
            return this->fullyQualifiedName();
        }

        std::stringstream s;

        assert(this->isCallable());

        if (this->flavor() == Flavor::Closure) {
            s << "{";
        } else {
            s << "(";
        }

        this->eachParameterWithLast([&] (const TypeReference& type, bool last) {
            s << type.str();
            if (!last) {
                s << ", ";
            }
        });

        s << "; ";
        s << this->returnType().str();

        if (this->flavor() == Flavor::Closure) {
            s << "}";
        } else {
            s << ")";
        }

        return s.str();
    }

    DataType::Flavor DataType::flavor() const {
        return _type;
    }

    void DataType::setFlavor(const Flavor& value) {
        _type = value;
    }

    std::string DataType::fullyQualifiedName() const {
        if (this->isNamespaced()) {
            return this->namespacePrefix() + "_3_" + this->name();
        }

        return this->name();
    }

    std::string DataType::cSourceName() const {
        if (this->flavor() == Flavor::Closure) {
            return "three_closure_t";
        } else if (this->name() == "Int") {
            return "int";
        } else if (this->name() == "Natural") {
            return "unsigned int";
        } else if (this->name() == "Natural:32") {
            return "uint32_t";
        } else if (this->name() == "Int:64") {
            return "int64_t";
        } else if (this->name() == "Char") {
            return "char";
        } else if (this->name() == "Void") {
            return "void";
        }

        return this->fullyQualifiedName();
    }

    bool DataType::cSourcePrependStructKeyword() const {
        return _prependStructKeyword;
    }
    void DataType::setCSourcePrependStructKeyword(bool prepend) {
        _prependStructKeyword = prepend;
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

    TypeReference DataType::childAtIndex(uint32_t index) const {
        return _children.at(index);
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

    void DataType::eachParameterWithLast(std::function<void (const TypeReference&, bool)> func) const {
        assert(this->isCallable());
        uint32_t lastIndex = this->childCount() - 1;

        this->eachChild([=] (const TypeReference& child, uint32_t index) {
            func(child, index == lastIndex);
        });
    }
}
