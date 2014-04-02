#include "BinaryAndOperatorNode.h"

namespace Three {
    std::string BinaryAndOperatorNode::nodeName() const {
        return "Binary And Operator";
    }

    void BinaryAndOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
