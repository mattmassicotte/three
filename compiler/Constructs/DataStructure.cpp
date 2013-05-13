#include "DataStructure.h"

namespace Language {
    DataStructure::DataStructure(const std::string& name) : _name(name) {
    }

    void DataStructure::setName(const std::string& string) {
        _name = string;
    }

    std::string DataStructure::name() const {
        return _name;
    }
}
