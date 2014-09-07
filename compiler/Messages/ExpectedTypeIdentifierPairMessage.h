#pragma once

#include "Message.h"

namespace Three {
    class ExpectedTypeIdentifierPairMessage : public Message {
    public:
        ExpectedTypeIdentifierPairMessage(const std::string& identifier);

        std::string name() const;
        std::string message() const;

    private:
        std::string _identifier;
    };
}
