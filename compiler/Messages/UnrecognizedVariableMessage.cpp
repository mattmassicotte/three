#include "UnrecognizedVariableMessage.h"

namespace Three {
    UnrecognizedVariableMessage::UnrecognizedVariableMessage(const std::string& identifier) : _identifier(identifier) {
    }

    std::string UnrecognizedVariableMessage::name() const {
        return "Unrecognized Variable";
    }

    std::string UnrecognizedVariableMessage::message() const {
        return "The identifier '" + _identifier + "' isn't defined in this scope.";
    }
}
