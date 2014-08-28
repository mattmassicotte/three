#pragma once

#include "../UnaryOperatorNode.h"

namespace Three {
    class DereferenceOperatorNode : public UnaryOperatorNode {
    public:
        NewDataType dataType() const;
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
