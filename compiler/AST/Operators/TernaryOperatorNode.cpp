#include "TernaryOperatorNode.h"

#include <assert.h>

namespace Three {
    void TernaryOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void TernaryOperatorNode::codeGen(CSourceContext& context) {
        assert(this->childCount() == 3);

        if (this->op() == "?") {
            this->childAtIndex(0)->codeGen(context);
            context << " ? ";
            this->childAtIndex(1)->codeGen(context);
            context << " : ";
            this->childAtIndex(2)->codeGen(context);
            return;
        }

        assert(0 && "codegen for ternary operator not supported");
    }
}
