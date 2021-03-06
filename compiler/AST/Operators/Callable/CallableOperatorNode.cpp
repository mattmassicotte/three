#include "CallableOperatorNode.h"
#include "compiler/constructs/DataType.h"
#include "compiler/Parser/Parser.h"
#include "compiler/AST/Control/ClosureNode.h"

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

        // Here we've ended including parsing the close-paren. But, we
        // could encounter a tailing closure, so check for that.

        if (parser.helper()->peek().type() == Token::Type::KeywordClosure) {
            node->addChild(ClosureNode::parse(parser));
        }

        return success;
    }

    CallableOperatorNode::CallableOperatorNode() : _receiver(nullptr) {
        
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
        assert(node && "Node cannot be null");
        assert(_receiver != node && "Cannot set receiver to be itself");

        if (_receiver) {
            delete _receiver;
        }

        _receiver = node;
    }

    DataType CallableOperatorNode::receiverDataType() const {
        return _receiver->dataType();
    }

    DataType CallableOperatorNode::dataType() const {
        return this->receiverDataType().returnType();
    }
}
