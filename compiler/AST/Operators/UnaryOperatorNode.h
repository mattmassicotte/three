#pragma once

#include "OperatorNode.h"

namespace Three {
    class UnaryOperatorNode : public OperatorNode {
    public:
        void codeGen(CSourceContext& context);
    };
}
