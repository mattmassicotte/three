#include "AtomicNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ASTNode* AtomicNode::parse(Parser& parser) {
        // TODO: this should take over for the special-casing doing in AtomicStatement
        return nullptr;
    }

    void AtomicNode::parseOrdering(Parser& parser, AtomicNode* node) {
        if (!parser.nextIf(":")) {
            node->_ordering = AtomicNode::Ordering::SequentiallyConsistent;
            return;
        }

        assert(parser.peek().type() == Token::Type::Identifier);
        std::string orderName = parser.next().str();

        if (orderName == "ordered") {
            node->_ordering = AtomicNode::Ordering::SequentiallyConsistent;
        } else if (orderName == "unordered") {
            node->_ordering = AtomicNode::Ordering::None;
        } else {
            node->_ordering = AtomicNode::Ordering::Undefined;
            assert(0 && "Unrecognized memory ordering for atomic statement");
        }
    }

    AtomicNode::Ordering AtomicNode::ordering() const {
        return _ordering;
    }

    std::string AtomicNode::c11MemoryOrderString() const {
        switch (this->ordering()) {
            case None:
                return "memory_order_relaxed";
            case Consume:
                return "memory_order_consume";
            case Acquire:
                return "memory_order_acquire";
            case Release:
                return "memory_order_release";
            case AcquireRelease:
                return "memory_order_acq_rel";
            case SequentiallyConsistent:
                return "memory_order_seq_cst";
            default:
                break;
        }

        assert(0 && "Unhandled memory ordering type");

        return "";
    }
}
