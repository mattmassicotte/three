#include "NotEqualOperatorNode.h"

namespace Three {
    std::string NotEqualOperatorNode::nodeName() const {
        return "Not-Equal Operator";
    }

    void NotEqualOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
