#include "RootNode.h"

namespace Three {
    std::string RootNode::name() const {
        return "Root";
    }

    void RootNode::codeGenCSource(CSourceContext& context) {
        this->codeGenCSourceForChildren(context);
    }
}
