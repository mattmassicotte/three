#pragma once

#include "../BinaryOperatorNode.h"

namespace Three {
    class AdditionOperatorNode : public BinaryOperatorNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
