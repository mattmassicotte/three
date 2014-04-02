#include "DereferenceOperatorNode.h"

namespace Three {
    std::string DereferenceOperatorNode::nodeName() const {
        return "Dereference Operator";
    }

    void DereferenceOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
