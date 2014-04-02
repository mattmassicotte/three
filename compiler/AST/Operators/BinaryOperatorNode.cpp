#include "BinaryOperatorNode.h"

#include <assert.h>

namespace Three {
    std::string BinaryOperatorNode::nodeName() const {
        return "Binary Operator";
    }

    void BinaryOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
