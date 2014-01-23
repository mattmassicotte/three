#include "OperatorNode.h"
#include "MemberAccessNode.h"
#include "IndexerNode.h"
#include "UnaryOperatorNode.h"
#include "BinaryOperatorNode.h"
#include "TernaryOperatorNode.h"
#include "FunctionCallOperatorNode.h"
#include "FunctionCallNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ASTNode* OperatorNode::parse(Parser& parser, ASTNode* left, uint32_t precedence) {
        OperatorNode* node = nullptr;

        assert(parser.peek().isOperator());

        if (parser.peek().str() == "?" || parser.peek().str() == "cas") {
            node = new TernaryOperatorNode();
        } else {
            node = new BinaryOperatorNode();
        }

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

        OperatorNode* node = new UnaryOperatorNode();
        node->setOp(parser.next().str());

        // unary operators can only have secondary expressions
        // as arguments (identifiers, unary operators)

        node->addChild(parser.parsePrimaryExpression());

        return node;
    }

    ASTNode* OperatorNode::parseTailing(Parser& parser, ASTNode* leftNode) {
        // possible tailing operators are:
        // .
        // ->
        // (
        // [

        while (true) {
            std::string op = parser.peek().str();

            assert(leftNode);

            if (op == "." || op == "->" || op == "[") {
                leftNode = OperatorNode::parseSingleTailing(parser, leftNode);
            } else if (op == "(") {
                leftNode = Three::FunctionCallOperatorNode::parse(parser, leftNode);
            } else {
                break;
            }
        }

        return leftNode;
    }

    ASTNode* OperatorNode::parseSingleTailing(Parser& parser, ASTNode* leftNode) {
        std::string op = parser.peek().str();

        // construct and parse the indexer operator
        if (op == "[") {
            return Three::IndexerNode::parse(parser, leftNode);
        }

        // check for a method invocation
        if (op == ".") {
            if (leftNode->nodeType().indirectionDepth() == 1) {
                parser.next();
                return FunctionCallNode::parseMethod(parser, leftNode);
            }
        }

        // member access operators
        if (op == "->" || op == ".") {
            return Three::MemberAccessNode::parse(parser, leftNode);
        }

        assert(0 && "Parsing a tailing operator failed");
        return leftNode;
    }

    TypeReference OperatorNode::nodeType() const {
        // TODO: is this correct?
        return this->childAtIndex(0)->nodeType();
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
}
