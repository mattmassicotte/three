#include "ImportFailureMessage.h"

namespace Three {
    ImportFailureMessage::ImportFailureMessage(const std::string& importName) : _importName(importName) {
    }

    std::string ImportFailureMessage::name() const {
        return "Import Failure";
    }

    std::string ImportFailureMessage::message() const {
        return "Unable to import '" + _importName + "'";
    }
}
