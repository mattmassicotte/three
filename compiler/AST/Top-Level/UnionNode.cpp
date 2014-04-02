#include "UnionNode.h"

namespace Three {
    std::string UnionNode::nodeName() const {
        return "Union";
    }

    void UnionNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void UnionNode::setParsedIntegerSpecifier(NewParser& parser, uint32_t value) {
        assert(0 && "Message: Integer specifiers not used for unions");
    }
}
