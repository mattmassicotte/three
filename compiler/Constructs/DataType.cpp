#include "DataType.h"
#include "../Parser.h"

#include <assert.h>

namespace Three {
    DataType::DataType(const Flavor& type) : _type(type), _prependStructKeyword(false) {
    }

    DataType::DataType(const Flavor& type, const std::string& name) : _type(type), _prependStructKeyword(false) {
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

        this->eachParameterWithLast([&] (const TypeReference& type, const std::string& name, bool last) {
            s << type.str() << " " << name;
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
        } else if (this->name() == "Bool") {
            return "bool";
        } else if (this->name() == "Void") {
            return "void";
        }

        return this->fullyQualifiedName();
    }

    std::string DataType::cHeader() const {
        if (this->flavor() == Flavor::Closure) {
            return "three/runtime/closure.h";
        } else if (this->name() == "Bool") {
            return "stdbool.h";
        } else if (this->name() == "Natural:32") {
            return "stdint.h";
        } else if (this->name() == "Int:64") {
            return "stdint.h";
        }

        return "";
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

    void DataType::addChild(const TypeReference& value, const std::string& name) {
        _children.push_back(value);
        _childNames.push_back(name);
    }

    void DataType::eachChild(std::function<void (const TypeReference&, const std::string&, uint32_t)> func) const {
        assert(_children.size() == _childNames.size());

        for (uint32_t i = 0; i < _children.size(); ++i) {
            func(_children[i], _childNames[i], i);
        }
    }

    TypeReference DataType::childAtIndex(uint32_t index) const {
        return _children.at(index);
    }

    TypeReference DataType::childForName(const std::string& name) const {
        assert(_children.size() == _childNames.size());

        for (uint32_t i = 0; i < _children.size(); ++i) {
            if (_childNames[i] == name) {
                return _children[i];
            }
        }

        assert(0 && "Child name not found for DataType");

        return TypeReference();
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

    void DataType::eachParameterWithLast(std::function<void (const TypeReference&, const std::string&, bool)> func) const {
        assert(this->isCallable());
        uint32_t lastIndex = this->childCount() - 1;

        this->eachChild([=] (const TypeReference& child, const std::string& name, uint32_t index) {
            func(child, name, index == lastIndex);
        });
    }
}
