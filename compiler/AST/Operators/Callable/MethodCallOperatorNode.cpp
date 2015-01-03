#include "MethodCallOperatorNode.h"
#include "compiler/Parser/Parser.h"

namespace Three {
    MethodCallOperatorNode* MethodCallOperatorNode::parse(Parser& parser, ASTNode* receiver) {
        assert(parser.helper()->nextIf(Token::Type::OperatorDot));

        MethodCallOperatorNode* node = new MethodCallOperatorNode();

        node->setReceiver(receiver);

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: method name should be an identifier");
        }

        if (!receiver->dataType().isPointer()) {
            assert(0 && "Message: Method receiver type must be a pointer");
        }

        node->_name = parser.helper()->nextStr();

        if (!CallableOperatorNode::parseArguments(parser, node)) {
            assert(0 && "Message: Unable to parse method arguments");
        }

        return node;
    }

    std::string MethodCallOperatorNode::nodeName() const {
        return "Method Call Operator";
    }

    void MethodCallOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string MethodCallOperatorNode::name() const {
        return _name;
    }
}
