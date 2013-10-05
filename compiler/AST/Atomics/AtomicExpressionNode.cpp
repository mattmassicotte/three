#include "AtomicExpressionNode.h"
#include "../Operators/OperatorNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    AtomicExpressionNode* AtomicExpressionNode::parse(Parser& parser, std::string type) {
        AtomicExpressionNode* node = new AtomicExpressionNode();

        assert(parser.next().type() == Token::Type::KeywordAtomic);

        AtomicNode::parseOrdering(parser, node);

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->addChild(parser.parseExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string AtomicExpressionNode::c11AtomicFunctionForOperator(const std::string& op) {
        if (op == "+=") {
            return "atomic_fetch_add_explicit";
        } else if (op == "-=") {
            return "atomic_fetch_sub_explicit";
        }

        return "";
    }

    std::string AtomicExpressionNode::name() const {
        return "AtomicExpression";
    }

    void AtomicExpressionNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("three/runtime/stdatomic.h");

        // we need to inspect the operation inside the expression to figure out
        // what code we actually need to emit
        assert(this->childCount() == 1);

        OperatorNode* op = dynamic_cast<OperatorNode*>(this->childAtIndex(0));

        std::string functionName = AtomicExpressionNode::c11AtomicFunctionForOperator(op->op());

        if (functionName != "") {
            context << functionName << "(";
            context << "(_Atomic(";
            op->nodeType().codeGenCSource(context.current(), "");
            context << ")*)";
            context << "&";
            op->childAtIndex(0)->codeGenCSource(context);
            context << ", ";
            op->childAtIndex(1)->codeGenCSource(context);
            context << ", ";
            context << this->c11MemoryOrderString();
            context << ")";
        } else if (op->op() == ">") {
            context << "atomic_load_explicit(";
            context << "(_Atomic(";
            op->nodeType().codeGenCSource(context.current(), "");
            context << ")*)";
            context << "&";
            op->childAtIndex(0)->codeGenCSource(context);
            context << ", ";
            context << this->c11MemoryOrderString();
            context << ")";
            context << " > ";
            op->childAtIndex(1)->codeGenCSource(context);
        } else if (op->op() == "cas") {
            context << "atomic_compare_exchange_strong_explicit(";
            context << "(_Atomic(";
            op->nodeType().codeGenCSource(context.current(), "");
            context << ")*)";
            context << "&";
            op->childAtIndex(0)->codeGenCSource(context);
            context << ", &(";
            op->childAtIndex(1)->codeGenCSource(context);
            context << "), ";
            op->childAtIndex(2)->codeGenCSource(context);
            context << ", ";
            context << this->c11MemoryOrderString();
            context << ", ";
            context << this->c11MemoryOrderString();
            context << ")";
        }

        if (this->statement()) {
            context.current()->print(";");
        }
    }
}
