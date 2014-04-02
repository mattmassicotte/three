#include "ReferencedVariableNode.h"

namespace Three {
    std::string ReferencedVariableNode::nodeName() const {
        return "Referenced Variable";
    }

    void ReferencedVariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
