#pragma once

#include "Message.h"

namespace Three {
    class ImportFailureMessage : public Message {
    public:
        ImportFailureMessage(const std::string& importName);

        std::string name() const;
        std::string message() const;

    private:
        std::string _importName;
    };
}
