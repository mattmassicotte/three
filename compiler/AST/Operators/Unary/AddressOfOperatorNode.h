#pragma once

#include "../UnaryOperatorNode.h"

namespace Three {
    class AddressOfOperatorNode : public UnaryOperatorNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
