#pragma once

#include "../BinaryOperatorNode.h"

namespace Three {
    class BinaryAndOperatorNode : public BinaryOperatorNode {
    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
