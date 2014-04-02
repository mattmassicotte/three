#pragma once

#include "Message.h"

namespace Three {
    class EmptyInputMessage : public Message {
    public:
        std::string name() const;
        std::string message() const;
    };
}
