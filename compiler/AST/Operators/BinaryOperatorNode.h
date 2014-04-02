#pragma once

#include "OperatorNode.h"

namespace Three {
    class BinaryOperatorNode : public OperatorNode {
    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
