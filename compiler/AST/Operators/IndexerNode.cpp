#include "IndexerNode.h"

namespace Three {
    std::string IndexerNode::name() const {
        return "Indexer";
    }

    void IndexerNode::codeGenCSource(CSourceContext& context) {
        this->childAtIndex(0)->codeGenCSource(context);

        context << "[";

        // index expression
        this->childAtIndex(1)->codeGenCSource(context);

        context << "]";
    }
}
