#include "OperatorNode.h"
#include "../../Parser.h"
#include "MemberAccessNode.h"
#include "IndexerNode.h"

#include <assert.h>

namespace Language {
    ASTNode* OperatorNode::parse(Parser& parser, ASTNode* left, uint32_t precedence) {
        OperatorNode* node = new OperatorNode();

        assert(parser.peek().type() == Token::Type::Operator); // move past the current operator
        node->setOp(parser.next().str());

        node->addChild(left);
        node->addChild(parser.parseExpression(precedence));

        if (node->op() == "?" || node->op() == "cas") {
            assert(parser.next().type() == Token::Type::PunctuationColon);

            node->addChild(parser.parseExpression(precedence));
        }

        return node;
    }

    ASTNode* OperatorNode::parseUnary(Parser& parser) {
        assert(parser.peek().isUnaryOperator());

        OperatorNode* node = new OperatorNode();
        node->setOp(parser.next().str());

        // unary operators can only have secondary expressions
        // as arguments (identifiers, unary operators)

        node->addChild(parser.parsePrimaryExpression());

        return node;
    }

    std::string OperatorNode::name() const {
        return "Operator";
    }

    std::string OperatorNode::str() const {
        return "Operator: " + this->op();
    }

    std::string OperatorNode::op() const {
        return _operator;
    }

    void OperatorNode::setOp(const std::string& string) {
        _operator = string;
    }

    bool OperatorNode::ternary() const {
        return (this->op() == "?") || (this->op() == "cas");
    }

    void OperatorNode::codeGenCSource(CSourceContext& context) {
        if (!this->statement()) {
            context.current()->print("(");
        }

        if (this->op() == "||=") {
            context << "!";
            this->childAtIndex(0)->codeGenCSource(context);
            context << " ? (";
            this->childAtIndex(0)->codeGenCSource(context);
            context << " = ";
            this->childAtIndex(1)->codeGenCSource(context);
            context << ") : ";
            this->childAtIndex(0)->codeGenCSource(context);
        } else if (this->op() == "&&=") {
            this->childAtIndex(1)->codeGenCSource(context);
            context << " ? (";
            this->childAtIndex(0)->codeGenCSource(context);
            context << " = ";
            this->childAtIndex(1)->codeGenCSource(context);
            context << ") : ";
            this->childAtIndex(0)->codeGenCSource(context);
        } else if (this->op() == "?") {
            this->childAtIndex(0)->codeGenCSource(context);
            context << " ? ";
            this->childAtIndex(1)->codeGenCSource(context);
            context << " : ";
            this->childAtIndex(2)->codeGenCSource(context);
        } else if (this->op() == "cas") {
            context << "(";
            this->childAtIndex(0)->codeGenCSource(context);
            context << " == ";
            this->childAtIndex(1)->codeGenCSource(context);
            context << ") ? (";
            this->childAtIndex(0)->codeGenCSource(context);
            context << " = ";
            this->childAtIndex(2)->codeGenCSource(context);
            context << ") : ";
            this->childAtIndex(1)->codeGenCSource(context);
        } else if ((this->op() == "&" || this->op() == "*") && this->childCount() == 1) {
            context.current()->print(this->op());
            this->childAtIndex(0)->codeGenCSource(context);
        } else {
            this->childAtIndex(0)->codeGenCSource(context);
            context.current()->print(" ");
            context.current()->print(this->op());
            context.current()->print(" ");
            this->childAtIndex(1)->codeGenCSource(context);
        }

        if (this->statement()) {
            context.current()->print(";");
        } else {
            context.current()->print(")");
        }
    }
}
