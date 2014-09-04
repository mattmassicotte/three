#include "UnableToCompleteParseMessage.h"

namespace Three {
    std::string UnableToCompleteParseMessage::name() const {
        return "Unable To Complete Parse";
    }

    std::string UnableToCompleteParseMessage::message() const {
        return "The input file could not be completely parsed";
    }
}
