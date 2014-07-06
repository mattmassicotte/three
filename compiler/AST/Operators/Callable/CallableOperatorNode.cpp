#include "CallableOperatorNode.h"
#include "compiler/constructs/NewDataType.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    bool CallableOperatorNode::parseArguments(Parser& parser, CallableOperatorNode* node) {
        assert(parser.helper()->nextIf(Token::Type::PunctuationOpenParen));

        // each argument is an expression, and cannot be a tuple
        bool success = parser.helper()->parseUntil(true, [&] (const Token& token) {
            if (token.type() == Token::Type::PunctuationCloseParen) {
                return true;
            }

            node->addChild(parser.parseExpression());

            // if we run into a comma, parse it and return false
            return !parser.helper()->nextIf(",");
        });

        // here we've ended including parsing the close-paren

        return success;
    }

    CallableOperatorNode::~CallableOperatorNode() {
        assert(_receiver);
        delete _receiver;
    }

    std::string CallableOperatorNode::nodeName() const {
        return "Callable Operator";
    }

    void CallableOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    ASTNode* CallableOperatorNode::receiver() const {
        return _receiver;
    }

    void CallableOperatorNode::setReceiver(ASTNode* node) {
        assert(_receiver != node);

        if (_receiver) {
            delete _receiver;
        }

        _receiver = node;
    }

    NewDataType CallableOperatorNode::receiverDataType() const {
        return _receiver->dataType();
    }

    NewDataType CallableOperatorNode::dataType() const {
        return this->receiverDataType();//.returnAtIndex(0);
    }
}
