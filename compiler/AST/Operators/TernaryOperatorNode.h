#pragma once

#include "OperatorNode.h"

namespace Three {
    class TernaryOperatorNode : public OperatorNode {
    public:
        void accept(ASTVisitor& visitor);
    };
}
