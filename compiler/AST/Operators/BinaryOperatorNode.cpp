#include "BinaryOperatorNode.h"

#include <assert.h>

namespace Three {
    void BinaryOperatorNode::codeGen(CSourceContext& context) {
        assert(this->childCount() == 2);

        this->childAtIndex(0)->codeGen(context);
        context << " " << this->op() << " ";
        this->childAtIndex(1)->codeGen(context);
    }
}
