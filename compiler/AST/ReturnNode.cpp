#include "ReturnNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    ReturnNode* ReturnNode::parse(Parser& parser) {
        ReturnNode* node = new ReturnNode();

        assert(parser.peek().type() == Token::Type::KeywordReturn);
        parser.next();

        switch (parser.peek().type()) {
            case Token::Type::Newline:
            case Token::Type::KeywordIf: // for tailing ifs
                break;
            default:
                node->addChild(parser.parseExpression());
                break;
        }

        node->setStatement(true);

        return node;
    }

    std::string ReturnNode::name() const {
        return "Return";
    }

    void ReturnNode::codeGenCSource(CSourceContext& context) {
        context << "return";

        assert(this->childCount() < 2);
        if (this->childCount() == 1) {
            context << " "; // space between keyword and expression
            this->childAtIndex(0)->codeGenCSource(context);
        }

        context << ";";
    }
}
