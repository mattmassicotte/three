#include "UnaryOperatorNode.h"

#include <assert.h>

namespace Three {
    void UnaryOperatorNode::codeGen(CSourceContext& context) {
        assert(this->childCount() == 1);

        context << "(";
        context << this->op();
        this->childAtIndex(0)->codeGen(context);
        context << ")";
    }
}
