#include "ExpectedTypeIdentifierPairMessage.h"

namespace Three {
    ExpectedTypeIdentifierPairMessage::ExpectedTypeIdentifierPairMessage(const std::string& identifier) : _identifier(identifier) {
    }

    std::string ExpectedTypeIdentifierPairMessage::name() const {
        return "Expected Type-Identifier Pair";
    }

    std::string ExpectedTypeIdentifierPairMessage::message() const {
        return "Expected either a type-identifier pair or an identifier.";
    }
}
