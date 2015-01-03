#include "OperatorNode.h"
#include "MemberAccessNode.h"
#include "IndexerNode.h"
#include "UnaryOperatorNode.h"
#include "BinaryOperatorNode.h"
#include "TernaryOperatorNode.h"
#include "Callable/FunctionCallOperatorNode.h"
#include "Callable/MethodCallOperatorNode.h"
#include "compiler/AST/Variables/VariableNode.h"
#include "compiler/Parser/Parser.h"

#include "Operators.h"

#include <assert.h>

namespace Three {
    ASTNode* OperatorNode::parse(Parser& parser, ASTNode* left, uint32_t precedence) {
        OperatorNode* node = OperatorNode::createOperator(parser);
        if (!node) {
            assert(0 && "Message: operator expected!");
        }

        node->setOp(parser.helper()->nextStr());

        node->addChild(left);
        node->addChild(parser.parseExpression(precedence));

        if (node->ternary()) {
            if (parser.helper()->next().type() != Token::Type::PunctuationColon) {
                assert(0 && "Message: Expecting colon in ternary operator");
            }

            node->addChild(parser.parseExpression(precedence));
        }

        return node;
    }

    ASTNode* OperatorNode::parseUnary(Parser& parser) {
        if (!parser.helper()->peek().isUnaryOperator()) {
            assert(0 && "Message: Unary operator expected");
        }

        OperatorNode* node = OperatorNode::createOperator(parser, true);
        node->setOp(parser.helper()->nextStr());

        // unary operators can only have secondary expressions
        // as arguments (identifiers, unary operators)

        node->addChild(parser.parseExpressionElement());

        return node;
    }

    ASTNode* OperatorNode::parseTailing(Parser& parser, ASTNode* leftNode) {
        // possible tailing operators are:
        // .
        // ->
        // (
        // [

        for (;;) {
            switch (parser.helper()->peek().type()) {
                case Token::Type::OperatorDot:
                case Token::Type::OperatorArrow:
                case Token::Type::PunctuationOpenBracket:
                    leftNode = OperatorNode::parseSingleTailing(parser, leftNode);
                    break;
                case Token::Type::PunctuationOpenParen:
                    leftNode = FunctionCallOperatorNode::parse(parser, leftNode);
                    break;
                default:
                    return leftNode;
            }

            // if we fail to parse anything, kill the loop
            if (!leftNode) {
                break;
            }
        }

        return nullptr;
    }

    ASTNode* OperatorNode::parseSingleTailing(Parser& parser, ASTNode* leftNode) {
        switch (parser.helper()->peek().type()) {
            case Token::Type::PunctuationOpenBracket:
                return IndexerNode::parse(parser, leftNode);
            case Token::Type::OperatorDot:
                if (leftNode->dataType().isPointer()) {
                    return MethodCallOperatorNode::parse(parser, leftNode);
                }
                // intentional fallthrough if not a method call
            case Token::Type::OperatorArrow:
                return MemberAccessNode::parse(parser, leftNode);
            default:
                break;
        }

        assert(0 && "Message: Unable to parse tailing operator");

        return leftNode;
    }

    OperatorNode* OperatorNode::createOperator(Parser& parser, bool unary) {
        if (unary) {
            switch (parser.helper()->peek().type()) {
                case Token::Type::OperatorStar:      return new DereferenceOperatorNode();
                case Token::Type::OperatorAmpersand: return new AddressOfOperatorNode();
                case Token::Type::OperatorMinus:     return new UnaryMinusOperatorNode();
                case Token::Type::OperatorNot:       return new NotOperatorNode();
                case Token::Type::OperatorBinaryNot:
                case Token::Type::OperatorIncrement:
                case Token::Type::OperatorDecrement:
                    break;
                default:
                    assert(0);
            }
        }

        switch (parser.helper()->peek().type()) {
            case Token::Type::OperatorStar:             return new MultiplicationOperatorNode();
            case Token::Type::OperatorAmpersand:        return new BinaryAndOperatorNode();
            case Token::Type::OperatorMinus:            return new SubtractionOperatorNode();
            case Token::Type::OperatorIncrement:        return new PlainBinaryOperatorNode("Increment Operator");
            case Token::Type::OperatorDecrement:        return new PlainBinaryOperatorNode("Decrement Operator");
            case Token::Type::OperatorPlus:             return new AdditionOperatorNode();
            case Token::Type::OperatorDivide:           return new DivisionOperatorNode();
            case Token::Type::OperatorMod:              return new PlainBinaryOperatorNode("Modulus Operator");
            case Token::Type::OperatorDot:              return new PlainBinaryOperatorNode("Dot Operator");
            case Token::Type::OperatorArrow:            return new PlainBinaryOperatorNode("Arrow Operator");
            case Token::Type::OperatorEqual:            return new PlainBinaryOperatorNode("Equal Operator");
            case Token::Type::OperatorDeepEqual:        return new PlainBinaryOperatorNode("Deep-Equal Operator");
            case Token::Type::OperatorCompare:          return new PlainBinaryOperatorNode("Compare Operator");
            case Token::Type::OperatorNotEqual:         return new NotEqualOperatorNode();
            case Token::Type::OperatorGreaterThan:      return new PlainBinaryOperatorNode("Greater-Than Operator");
            case Token::Type::OperatorLessThan:         return new PlainBinaryOperatorNode("Less-Than Operator");
            case Token::Type::OperatorGreaterOrEqual:   return new PlainBinaryOperatorNode("Greater-or-Equal Operator");
            case Token::Type::OperatorLessOrEqual:      return new PlainBinaryOperatorNode("Less-or-Equal Operator");
            case Token::Type::OperatorLogicalAnd:       return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorLogicalOr:        return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorLogicalXor:       return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorNot:              return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorBinaryNot:        return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorBinaryOr:         return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorBinaryXor:        return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorLeftShift:        return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorRightShift:       return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorAssign:           return new AssignOperatorNode();
            case Token::Type::OperatorAddAssign:        return new PlainBinaryOperatorNode("Add-Assign Operator");
            case Token::Type::OperatorSubtractAssign:   return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorMultiplyAssign:   return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorDivideAssign:     return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorModAssign:        return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorBitwiseAndAssign: return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorBitwiseOrAssign:  return new PlainBinaryOperatorNode("Bitwise Or-Assign Operator");
            case Token::Type::OperatorBitwiseXorAssign: return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorLeftShiftAssign:  return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorRightShiftAssign: return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorLogicalAndAssign: return new PlainBinaryOperatorNode("");
            case Token::Type::OperatorLogicalOrAssign:  return new PlainBinaryOperatorNode("Or-Assign Operator");
            case Token::Type::OperatorQuestionMark:     return new TernaryConditionalOperatorNode();
            case Token::Type::OperatorCAS:              return new CASOperatorNode();
            default: break;
        }

        return nullptr;
    }

    DataType OperatorNode::dataType() const {
        assert(this->childCount() > 0);

        // TODO: is this correct?
        return this->childAtIndex(0)->dataType();
    }

    std::string OperatorNode::nodeName() const {
        return "Operator";
    }

    std::string OperatorNode::name() const {
        return "Operator!!!";
    }

    std::string OperatorNode::str() const {
        return this->nodeName();
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
