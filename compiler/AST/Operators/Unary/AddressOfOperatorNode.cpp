#include "AddressOfOperatorNode.h"

namespace Three {
    std::string AddressOfOperatorNode::nodeName() const {
        return "Address Of Operator";
    }

    void AddressOfOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
