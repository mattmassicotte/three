#pragma once

#include "../UnaryOperatorNode.h"

namespace Three {
    class DereferenceOperatorNode : public UnaryOperatorNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
