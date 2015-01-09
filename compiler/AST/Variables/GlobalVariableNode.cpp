#include "GlobalVariableNode.h"

namespace Three {
    std::string GlobalVariableNode::nodeName() const {
        return "Global Variable";
    }

    void GlobalVariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
