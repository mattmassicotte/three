#include "ReturnNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    ReturnNode* ReturnNode::parse(Parser& parser) {
        ReturnNode* node = new ReturnNode();

        assert(parser.peek().type() == Token::Type::KeywordReturn);
        parser.next();

        if (parser.peek().type() != Token::Type::Newline) {
            node->addChild(parser.parseExpression());
        }

        node->setStatement(true);

        return node;
    }

    std::string ReturnNode::name() const {
        return "Return";
    }

    void ReturnNode::codeGenCSource(CSourceContext& context) {
        context.print("return");

        assert(this->childCount() < 2);
        if (this->childCount() == 1) {
            context.print(" "); // space between keyword and expression
            this->childAtIndex(0)->codeGenCSource(context);
        }

        context.print(";");
    }
}
