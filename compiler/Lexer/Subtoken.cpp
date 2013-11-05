#include "Subtoken.h"

namespace Three {
    Subtoken::Subtoken() : _type(Type::Undefined) {
    }

    Subtoken::Subtoken(Subtoken::Type type) : _type(type) {
    }

    Subtoken::Subtoken(Subtoken::Type type, const std::string& value) : _type(type), _value(value) {
    }

    Subtoken::Type Subtoken::type() const {
        return _type;
    }

    std::string Subtoken::str() const {
        return _value;
    }

    std::string Subtoken::stringValue() const {
        // chop off the quotes
        return _value.substr(1, _value.length() - 2);
    }
}
