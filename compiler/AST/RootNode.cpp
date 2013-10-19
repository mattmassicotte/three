#include "RootNode.h"

namespace Three {
    std::string RootNode::name() const {
        return "Root";
    }

    void RootNode::codeGen(CSourceContext& context) {
        this->codeGenChildren(context);
    }
}
