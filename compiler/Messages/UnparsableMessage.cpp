#include "UnparsableMessage.h"

namespace Three {
    std::string UnparsableMessage::name() const {
        return "Unparsable Source";
    }

    std::string UnparsableMessage::message() const {
        return "The input file cannot be parsed";
    }
}
