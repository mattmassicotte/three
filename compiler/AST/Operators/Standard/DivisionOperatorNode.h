#pragma once

#include "../BinaryOperatorNode.h"

namespace Three {
    class DivisionOperatorNode : public BinaryOperatorNode {
    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
