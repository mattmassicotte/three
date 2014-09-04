#pragma once

#include "Message.h"

namespace Three {
    class UnableToCompleteParseMessage : public Message {
    public:
        std::string name() const;
        std::string message() const;
    };
}
