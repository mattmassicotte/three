#include "SelfVariableNode.h"

namespace Three {
    std::string SelfVariableNode::nodeName() const {
        return "Self Variable";
    }

    void SelfVariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
