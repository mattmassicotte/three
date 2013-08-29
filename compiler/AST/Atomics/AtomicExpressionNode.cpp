#include "AtomicExpressionNode.h"
#include "../Operators/OperatorNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    AtomicExpressionNode* AtomicExpressionNode::parse(Parser& parser, std::string type) {
        AtomicExpressionNode* node = new AtomicExpressionNode();

        if (type.size() == 0) {
            assert(parser.next().type() == Token::Type::KeywordAtomic);

            if (parser.nextIf(":")) {
                node->_type = parser.next().str();
            } else {
                node->_type = "ordered";
            }
        } else {
            node->_type = type;
        }

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->addChild(parser.parseExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string AtomicExpressionNode::name() const {
        return "AtomicExpression";
    }

    std::string AtomicExpressionNode::type() const {
        return _type;
    }

    void AtomicExpressionNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("three/runtime/stdatomic.h");

        // we need to inspect the operation inside the expression to figure out
        // what code we actually need to emit

        assert(this->childCount() == 1);

        OperatorNode* op = dynamic_cast<OperatorNode*>(this->childAtIndex(0));

        if (op->op() == "+=") {
            context << "atomic_fetch_add_explicit(";
            context << "&(";
            op->childAtIndex(0)->codeGenCSource(context);
            context << "), ";
            op->childAtIndex(1)->codeGenCSource(context);
            context << ", ";
            context << "memory_order_seq_cst";
            context << ")";
        } else if (op->op() == "cas") {
            context << "atomic_compare_exchange_strong_explicit(";
            context << "&(";
            op->childAtIndex(0)->codeGenCSource(context);
            context << "), &(";
            op->childAtIndex(1)->codeGenCSource(context);
            context << "), &(";
            op->childAtIndex(2)->codeGenCSource(context);
            context << "), ";
            context << "memory_order_seq_cst";
            context << ", ";
            context << "memory_order_seq_cst";
            context << ")";
        }
    }
}
