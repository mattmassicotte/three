#include "TernaryConditionalOperatorNode.h"

namespace Three {
    std::string TernaryConditionalOperatorNode::nodeName() const {
        return "Ternary Conditional Operator";
    }

    void TernaryConditionalOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
