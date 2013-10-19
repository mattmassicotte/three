#include "TernaryOperatorNode.h"

#include <assert.h>

namespace Three {
    void TernaryOperatorNode::codeGen(CSourceContext& context) {
        assert(this->childCount() == 3);

        this->childAtIndex(0)->codeGen(context);
        context << " " << this->op() << " ";
        this->childAtIndex(1)->codeGen(context);
    }
}
