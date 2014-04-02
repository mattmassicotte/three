#pragma once

#include "../BinaryOperatorNode.h"

namespace Three {
    class MultiplicationOperatorNode : public BinaryOperatorNode {
    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
