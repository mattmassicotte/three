#include "AtomicNode.h"
#include "../../Parser.h"
#include "AtomicExpressionNode.h"
#include "AtomicStatementNode.h"

#include <assert.h>

namespace Three {
    ASTNode* AtomicNode::parse(Parser& parser) {
        assert(parser.peek().type() == Token::Type::KeywordAtomic);

        // we need to check for an atomic statement
        // case #1 "atomic (...)"
        if (parser.peek(2).type() == Token::Type::PunctuationOpenParen) {
            return AtomicExpressionNode::parse(parser);
        }

        // case #1 "atomic:ordering (...)" where "ordering" != fallback
        if (parser.peek(2).type() == Token::Type::PunctuationColon) {
            if (parser.peek(3).str() != "fallback") {
                return AtomicExpressionNode::parse(parser);
            }
        }

        return AtomicStatementNode::parse(parser);
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
