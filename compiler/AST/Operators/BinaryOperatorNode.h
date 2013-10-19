#pragma once

#include "OperatorNode.h"

namespace Three {
    class BinaryOperatorNode : public OperatorNode {
    public:
        void codeGen(CSourceContext& context);
    };
}
