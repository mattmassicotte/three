#include "NotOperatorNode.h"

namespace Three {
    std::string NotOperatorNode::nodeName() const {
        return "Not Operator";
    }

    void NotOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
