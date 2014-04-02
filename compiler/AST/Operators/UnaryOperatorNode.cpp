#include "UnaryOperatorNode.h"

#include <assert.h>

namespace Three {
    std::string UnaryOperatorNode::nodeName() const {
        return "Unary Operator";
    }

    void UnaryOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
