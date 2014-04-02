#include "FunctionVariableNode.h"

namespace Three {
    std::string FunctionVariableNode::nodeName() const {
        return "Function Variable";
    }

    void FunctionVariableNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
