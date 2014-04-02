#include "UnaryMinusOperatorNode.h"

namespace Three {
    std::string UnaryMinusOperatorNode::nodeName() const {
        return "Unary Minus Operator";
    }

    void UnaryMinusOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
