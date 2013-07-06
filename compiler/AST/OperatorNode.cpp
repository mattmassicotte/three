#include "OperatorNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    ASTNode* OperatorNode::parse(Parser& parser, uint32_t precedence, ASTNode* leftOperand) {
        assert(leftOperand != NULL);
        assert(precedence > Token::NonPrecedence);

        while (true) {
            Token t = parser.peek();

            // if we've gotten to the end of a statement, we have nothing left to do here
            if (t.isStatementEnding()) {
                return leftOperand;
            }

            if (t.precedence() < precedence) {
                return leftOperand;
            }

            return OperatorNode::parseOperator(parser, leftOperand);
        }

        return leftOperand;
    }

    OperatorNode* OperatorNode::parseOperator(Parser& parser, ASTNode* leftOperand) {
        OperatorNode* node;
        std::string   op;

        node = new OperatorNode();
        
        while (parser.peek().type() == Token::Type::Operator) {
            op = op + parser.next().str();
        }

        node->setOp(op);

        node->addChild(leftOperand);
        node->addChild(parser.parseExpression());

        if (node->ternary()) {
            assert(parser.next().type() == Token::Type::PunctuationColon);
            node->addChild(parser.parseExpression());
        }

        return node;
    }

    std::string OperatorNode::name() const {
        return "Operator";
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

        this->childAtIndex(0)->codeGenCSource(context);
        context.current()->print(" ");
        context.current()->print(this->op());
        context.current()->print(" ");
        this->childAtIndex(1)->codeGenCSource(context);

        if (this->statement()) {
            context.current()->print(";");
        } else {
            context.current()->print(")");
        }
    }
}