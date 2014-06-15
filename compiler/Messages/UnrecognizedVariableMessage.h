#pragma once

#include "Message.h"

namespace Three {
    class UnrecognizedVariableMessage : public Message {
    public:
        UnrecognizedVariableMessage(const std::string& identifier);

        std::string name() const;
        std::string message() const;

    private:
        std::string _identifier;
    };
}
