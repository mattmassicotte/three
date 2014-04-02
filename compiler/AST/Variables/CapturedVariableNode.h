#pragma once

#include "VariableNode.h"

namespace Three {
    class CapturedVariableNode : public VariableNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
