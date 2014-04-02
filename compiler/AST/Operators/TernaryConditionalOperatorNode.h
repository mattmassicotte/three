#pragma once

#include "TernaryOperatorNode.h"

namespace Three {
    class TernaryConditionalOperatorNode : public TernaryOperatorNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
