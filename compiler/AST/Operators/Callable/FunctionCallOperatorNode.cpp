#include "FunctionCallOperatorNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/AST/Variables/VariableNode.h"

#include <assert.h>

namespace Three {
    FunctionCallOperatorNode* FunctionCallOperatorNode::parse(Parser& parser, ASTNode* receiver, ASTNode* firstArg) {
        assert(parser.helper()->peek().type() == Token::Type::PunctuationOpenParen);

        FunctionCallOperatorNode* node = new FunctionCallOperatorNode();

        std::cout << receiver->recursiveStr() << std::endl;

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
}
