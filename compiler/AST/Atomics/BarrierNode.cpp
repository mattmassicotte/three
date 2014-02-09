#include "BarrierNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    BarrierNode* BarrierNode::parse(Parser& parser) {
        BarrierNode* node = new BarrierNode();

        assert(parser.next().type() == Token::Type::KeywordBarrier);

        AtomicNode::parseOrdering(parser, node);

        return node;
    }

    std::string BarrierNode::name() const {
        return "Barrier";
    }

    void BarrierNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void BarrierNode::codeGen(CSourceContext& context) {
        context.declarations()->addHeader(false, "three/runtime/atomic.h");

        context << "atomic_thread_fence(";
        context << this->c11MemoryOrderString();
        context.current()->printLine(");");
    }
}
