#include "AdditionOperatorNode.h"

namespace Three {
    std::string AdditionOperatorNode::nodeName() const {
        return "Addition Operator";
    }

    void AdditionOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
