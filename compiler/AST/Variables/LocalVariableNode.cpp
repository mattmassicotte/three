#include "LocalVariableNode.h"

namespace Three {
    std::string LocalVariableNode::nodeName() const {
        return "Local Variable";
    }

    void LocalVariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
