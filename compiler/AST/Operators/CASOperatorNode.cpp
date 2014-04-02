#include "CASOperatorNode.h"

namespace Three {
    std::string CASOperatorNode::nodeName() const {
        return "CAS Operator";
    }

    void CASOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
