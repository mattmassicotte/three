#include "BarrierNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    BarrierNode* BarrierNode::parse(Parser& parser) {
        BarrierNode* node = new BarrierNode();

        assert(parser.next().type() == Token::Type::KeywordBarrier);

        AtomicNode::parseOrdering(parser, node);

        return node;
    }

    std::string BarrierNode::name() const {
        return "Barrier";
    }

    void BarrierNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("three/runtime/stdatomic.h");

        context << "atomic_thread_fence(" << this->c11MemoryOrderString() << ");";
    }
}
