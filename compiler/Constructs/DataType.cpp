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

    uint32_t DataType::childCount() const {
        return _children.size();
    }

    void DataType::eachChild(std::function<void (DataType*, uint32_t)> func) const {
        uint32_t index = 0;
        for (DataType* child : _children) {
            func(child, index);
            ++index;
        }
    }

    bool DataType::isFunction() const {
        return _type == Flavor::Function;
    }

    DataType* DataType::returnType() const {
        assert(this->isFunction());

        return _returnType;
    }

    void DataType::setReturnType(DataType* value) {
        assert(this->isFunction());
        assert(value);

        _returnType = value;
    }

    void DataType::eachParameterWithLast(std::function<void (DataType*, bool)> func) {
        assert(this->isFunction());
        uint32_t lastIndex = this->childCount() - 1;

        this->eachChild([=] (DataType* child, uint32_t index) {
            if (index == 0) {
                return;
            }

            func(child, index == lastIndex);
        });
    }
}
