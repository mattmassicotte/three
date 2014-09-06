#include "DuplicateFunctionMessage.h"

namespace Three {
    DuplicateFunctionMessage::DuplicateFunctionMessage(const std::string& identifier) : _identifier(identifier) {
    }

    std::string DuplicateFunctionMessage::name() const {
        return "Duplicate Function";
    }

    std::string DuplicateFunctionMessage::message() const {
        return "The function '" + _identifier + "' has already been defined.";
    }
}
