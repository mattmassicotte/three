#include "DereferenceOperatorNode.h"

namespace Three {
    DataType DereferenceOperatorNode::dataType() const {
        // we have to actually deref the type, to figure out
        // what this node resolves to
        DataType type = this->childAtIndex(0)->dataType();

        assert(type.kind() == DataType::Kind::Pointer);

        return type.subtypeAtIndex(0);
    }

    std::string DereferenceOperatorNode::nodeName() const {
        return "Dereference Operator";
    }

    void DereferenceOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
