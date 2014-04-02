#include "FunctionCallOperatorNode.h"
#include "compiler/Parser/NewParser.h"
#include "compiler/AST/Variables/VariableNode.h"

#include <assert.h>

namespace Three {
    FunctionCallOperatorNode* FunctionCallOperatorNode::parse(NewParser& parser, ASTNode* receiver, ASTNode* firstArg) {
        assert(parser.helper()->peek().type() == Token::Type::PunctuationOpenParen);

        FunctionCallOperatorNode* node = new FunctionCallOperatorNode();

        node->setReceiver(receiver);

        if (!CallableOperatorNode::parseArguments(parser, node)) {
            assert(0 && "Message: Unable to parse function arguments");
        }

        return node;
    }

    std::string FunctionCallOperatorNode::nodeName() const {
        return "Function Call Operator";
    }

    void FunctionCallOperatorNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    bool FunctionCallOperatorNode::receiverIsClosure() const {
        assert(this->receiver());

        return this->receiverDataType().kind() == NewDataType::Kind::Closure;
    }
}
