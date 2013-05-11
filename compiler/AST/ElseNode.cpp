#include "ElseNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    ElseNode* ElseNode::parse(Parser& parser) {
        ElseNode* node = new ElseNode();

        assert(parser.next().type() == Token::Type::KeywordElse);
        assert(parser.next().type() == Token::Type::Newline);

        // parse the body
        while (true) {
            if (parser.peek().type() == Token::Type::KeywordEnd) {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        assert(parser.next().type() == Token::Type::KeywordEnd);

        return node;
    }

    std::string ElseNode::name() const {
        return "Else";
    }

    void ElseNode::renderCCode(std::stringstream& stream, uint32_t indentation) {
        assert(0 && "ElseNode cannot be rendered to C directly");
    }
}
