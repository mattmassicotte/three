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

    void ReturnNode::renderCCode(std::stringstream& stream, uint32_t indentation) {
        stream << std::string(indentation*4, ' ');

        stream << "return";

        assert(this->childCount() < 2);
        if (this->childCount() == 1) {
            stream << " "; // space between keyword and expression
            this->childAtIndex(0)->renderCCode(stream, 0);
        }

        stream << ";" << std::endl;
    }
}
