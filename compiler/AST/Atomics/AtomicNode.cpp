#include "AtomicNode.h"
#include "AtomicExpressionNode.h"
#include "AtomicStatementNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    void AtomicNode::parseOrdering(Parser& parser, AtomicNode* node) {
        if (!parser.helper()->nextIf(Token::Type::PunctuationColon)) {
            node->_ordering = AtomicNode::Ordering::SequentiallyConsistent;
            return;
        }

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: an atomic ordering should be an identifier");
        }

        std::string orderName = parser.helper()->nextStr();

        if (orderName == "ordered") {
            node->_ordering = AtomicNode::Ordering::SequentiallyConsistent;
        } else if (orderName == "unordered") {
            node->_ordering = AtomicNode::Ordering::None;
        } else {
            node->_ordering = AtomicNode::Ordering::Undefined;
            assert(0 && "Message: Unrecognized memory ordering for atomic statement");
        }
    }

    bool AtomicNode::isAtAtomicExpression(Parser& parser) {
        if (parser.helper()->peek().type() != Token::Type::KeywordAtomic) {
            return false;
        }

        // It is important that this function work if whitespace filtering is off
        uint32_t peekDepth = 2;
        while (parser.helper()->peek(peekDepth).type() == Token::Type::Whitespace) {
            peekDepth++;
        }

        // case #1 "atomic (...)"
        if (parser.helper()->peek(peekDepth).type() == Token::Type::PunctuationOpenParen) {
            return true;
        }

        // case #2 "atomic:ordering (...)" where "ordering" != fallback
        if (parser.helper()->peek(peekDepth).type() != Token::Type::PunctuationColon) {
            return false;
        }

        return parser.helper()->peek(3).str() != "fallback";
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
