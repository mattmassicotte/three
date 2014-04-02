#include "EnsureNode.h"

#include <assert.h>

namespace Three {
    std::string EnsureNode::name() const {
        return "Ensure";
    }

    void EnsureNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
