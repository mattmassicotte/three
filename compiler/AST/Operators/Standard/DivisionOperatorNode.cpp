#include "DivisionOperatorNode.h"

namespace Three {
    std::string DivisionOperatorNode::nodeName() const {
        return "Division Operator";
    }

    void DivisionOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
