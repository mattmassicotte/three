#include "PlainBinaryOperatorNode.h"

namespace Three {
    PlainBinaryOperatorNode::PlainBinaryOperatorNode(const std::string& type) : _type(type) {
    }

    std::string PlainBinaryOperatorNode::nodeName() const {
        return _type;
    }

    void PlainBinaryOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
