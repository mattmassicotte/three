#include "SubtractionOperatorNode.h"

namespace Three {
    std::string SubtractionOperatorNode::nodeName() const {
        return "Subtraction Operator";
    }

    void SubtractionOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
