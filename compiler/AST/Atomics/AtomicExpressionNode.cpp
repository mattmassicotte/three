#include "AtomicExpressionNode.h"
#include "../Operators/OperatorNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    AtomicExpressionNode* AtomicExpressionNode::parse(Parser& parser, bool statement) {
        AtomicExpressionNode* node = new AtomicExpressionNode();

        assert(parser.next().type() == Token::Type::KeywordAtomic);

        AtomicNode::parseOrdering(parser, node);

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->addChild(parser.parseExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        node->setStatement(statement);

        return node;
    }

    std::string AtomicExpressionNode::c11AtomicFunctionForFullOperation(const std::string& op) {
        if (op == "+=") {
            return "atomic_fetch_add_explicit";
        } else if (op == "-=") {
            return "atomic_fetch_sub_explicit";
        }

        return "";
    }

    bool AtomicExpressionNode::c11AtomicFunctionIsLoadOperation(const std::string& op) {
        // >, >=, <, <=, ==
        return op == ">" || op == ">=" || op == "<" || op == "<=" || op == "==";
    }

    std::string AtomicExpressionNode::name() const {
        return "AtomicExpression";
    }

    void AtomicExpressionNode::codeGenAtomicVariable(CSourceContext& context, OperatorNode* op) {
        context << "(_Atomic(";
        op->nodeType().codeGen(context, "");
        context << ")*)";
        context << "&";

        op->childAtIndex(0)->codeGen(context);
    }

    void AtomicExpressionNode::codeGen(CSourceContext& context) {
        context.declarations()->addHeader(false, "three/runtime/atomic.h");

        // we need to inspect the operation inside the expression to figure out
        // what code we actually need to emit
        assert(this->childCount() == 1);

        OperatorNode* op = dynamic_cast<OperatorNode*>(this->childAtIndex(0));

        context.adjustCurrent(context.declarations(), [&] (CSource* source) {
            *source << "THREE_CHECK_ATOMIC(";
            op->nodeType().codeGen(context);
            source->printLine(");");
        });

        std::string functionName = AtomicExpressionNode::c11AtomicFunctionForFullOperation(op->op());
        if (functionName != "") {
            context << functionName;
            context << "(";
            this->codeGenAtomicVariable(context, op);
            context << ", ";
            op->childAtIndex(1)->codeGen(context);
            context << ", ";
            context << this->c11MemoryOrderString();
            context << ")";

            return;
        }

        if (AtomicExpressionNode::c11AtomicFunctionIsLoadOperation(op->op())) {
            context << "atomic_load_explicit(";
            this->codeGenAtomicVariable(context, op);
            context << ", ";
            context << this->c11MemoryOrderString();
            context << ") ";

            context << op->op();
            context << " ";

            op->childAtIndex(1)->codeGen(context);

            return;
        }

        assert(0 && "Atomic expression codegen failure");
    }
}
