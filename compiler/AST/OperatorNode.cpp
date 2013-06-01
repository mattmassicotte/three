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

            OperatorNode* node = new OperatorNode();

            std::cout << "Operation: " << parser.next().str() << std::endl;
            node->addChild(leftOperand);
            node->addChild(parser.parseExpression());

            return node;
        }
        return leftOperand;
    }

    std::string OperatorNode::name() const {
        return "Operator";
    }

    void OperatorNode::codeGenCSource(CSourceContext& context) {
        
    }
}
