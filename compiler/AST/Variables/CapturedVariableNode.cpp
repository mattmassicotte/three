#include "CapturedVariableNode.h"

namespace Three {
    std::string CapturedVariableNode::nodeName() const {
        return "Captured Variable";
    }

    void CapturedVariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
