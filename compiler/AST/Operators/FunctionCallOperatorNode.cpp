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

    ASTNode* FunctionCallOperatorNode::receiver() const {
        return _receiver;
    }

    void FunctionCallOperatorNode::codeGen(CSourceContext& context) {
        this->_receiver->codeGen(context);

        context << "(";

        this->eachChildWithLast([=, &context] (ASTNode* node, bool last) {
            node->codeGen(context);
            if (!last) {
                context << ", ";
            }
        });

        context << ")";
    }
}
