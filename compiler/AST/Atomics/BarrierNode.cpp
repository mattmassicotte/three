#include "BarrierNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    BarrierNode* BarrierNode::parse(Parser& parser) {
        BarrierNode* node = new BarrierNode();

        assert(parser.next().type() == Token::Type::KeywordBarrier);

        if (parser.nextIf(":")) {
            node->_type = parser.next().str();
        } else {
            node->_type = "ordered";
        }

        return node;
    }

    std::string BarrierNode::name() const {
        return "Barrier";
    }

    std::string BarrierNode::type() const {
        return _type;
    }

    std::string BarrierNode::c11MemoryOrderString() const {
        if (_type == "ordered") {
            return "memory_order_seq_cst";
        } else if (_type == "unordered") {
            return "memory_order_relaxed";
        }

        assert(0 && "Unhandled memory ordering type");

        return "unknown";
    }

    void BarrierNode::codeGenCSource(CSourceContext& context) {
        context << "atomic_thread_fence(" << this->c11MemoryOrderString() << ");";
    }
}
