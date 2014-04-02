#pragma once

#include "VariableNode.h"

namespace Three {
    class LocalVariableNode : public VariableNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
