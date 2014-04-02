#include "TernaryOperatorNode.h"

#include <assert.h>

namespace Three {
    void TernaryOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
