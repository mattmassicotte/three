#include "FunctionCallOperatorNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    FunctionCallOperatorNode* FunctionCallOperatorNode::parse(Parser& parser, ASTNode* receiver) {
        FunctionCallOperatorNode* node = new FunctionCallOperatorNode();

        node->_receiver = receiver;
        node->setOp("()");

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        // now, we need to parse the arguments, each of which
        // is an expression
        parser.parseUntil(true, [&] (const Token& token) {
            if (token.type() == Token::Type::PunctuationCloseParen) {
                return true;
            }

            node->addChild(parser.parseExpression());

            // if we run into a comma, parse it and return false
            return !parser.nextIf(",");
        });

        // here we've ended including parsing the close-paren

        if (parser.peek().type() == Token::Type::Newline) {
            node->setStatement(true);
        }

        return node;
    }

    void FunctionCallOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    ASTNode* FunctionCallOperatorNode::receiver() const {
        return _receiver;
    }

    TypeReference FunctionCallOperatorNode::receiverNodeType() const {
        return _receiver->nodeType();
    }

    bool FunctionCallOperatorNode::receiverIsClosure() const {
        assert(_receiver);

        if (!_receiver->nodeType().referencedType()) {
            return false;
        }

        return _receiver->nodeType().referencedType()->flavor() == DataType::Flavor::Closure;
    }

    void FunctionCallOperatorNode::codeGen(CSourceContext& context) {
        if (this->receiverIsClosure()) {
            context << "THREE_CALL_CLOSURE(";
            context << this->_receiver->nodeType().codeGenFunction("");
            context << ", ";
            this->_receiver->codeGen(context);

            if (this->childCount() > 0) {
                context << ", ";
            }
        } else {
            this->_receiver->codeGen(context);
            context << "(";
        }

        this->eachChildWithLast([=, &context] (ASTNode* node, bool last) {
            node->codeGen(context);
            if (!last) {
                context << ", ";
            }
        });

        context << ")";
    }
}
