#include "EmptyInputMessage.h"

namespace Three {
    std::string EmptyInputMessage::name() const {
        return "No Parsable Input";
    }

    std::string EmptyInputMessage::message() const {
        return "The input file produced had no parsable text";
    }
}
