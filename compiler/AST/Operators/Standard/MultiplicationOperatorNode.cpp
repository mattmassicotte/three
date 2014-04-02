#include "MultiplicationOperatorNode.h"

namespace Three {
    std::string MultiplicationOperatorNode::nodeName() const {
        return "Multiplication Operator";
    }

    void MultiplicationOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
