#pragma once

#include "../BinaryOperatorNode.h"

namespace Three {
    class AssignOperatorNode : public BinaryOperatorNode {
    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
