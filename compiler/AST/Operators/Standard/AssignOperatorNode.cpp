#include "AssignOperatorNode.h"

namespace Three {
    std::string AssignOperatorNode::nodeName() const {
        return "Assign Operator";
    }

    void AssignOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
