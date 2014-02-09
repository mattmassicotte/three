#pragma once

#include "OperatorNode.h"

namespace Three {
    class BinaryOperatorNode : public OperatorNode {
    public:
        void accept(ASTVisitor& visitor);

        void codeGen(CSourceContext& context);
    };
}
