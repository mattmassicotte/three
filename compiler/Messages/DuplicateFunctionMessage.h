#pragma once

#include "Message.h"

namespace Three {
    class DuplicateFunctionMessage : public Message {
    public:
        DuplicateFunctionMessage(const std::string& identifier);

        std::string name() const;
        std::string message() const;

    private:
        std::string _identifier;
    };
}
