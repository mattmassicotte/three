#include "ExpectedIdentifierMessage.h"

namespace Three {
    ExpectedIdentifierMessage::ExpectedIdentifierMessage(const std::string& value) : _value(value) {
    }

    std::string ExpectedIdentifierMessage::name() const {
        return "Expected Identifier";
    }

    std::string ExpectedIdentifierMessage::message() const {
        return "A valid identifier is expected here, not '" + _value + "'.";
    }
}
