#pragma once

#include "Message.h"

namespace Three {
    class ExpectedIdentifierMessage : public Message {
    public:
        ExpectedIdentifierMessage(const std::string& value);

        std::string name() const;
        std::string message() const;

    private:
        std::string _value;
    };
}
