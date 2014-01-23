#pragma once

#include "OperatorNode.h"

namespace Three {
    class UnaryOperatorNode : public OperatorNode {
    public:
        void accept(ASTVisitor& visitor);
        void codeGen(CSourceContext& context);
    };
}
