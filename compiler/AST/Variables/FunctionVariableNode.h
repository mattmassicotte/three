#pragma once

#include "VariableNode.h"

namespace Three {
    class FunctionVariableNode : public VariableNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
