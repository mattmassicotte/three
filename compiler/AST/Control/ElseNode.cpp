#include "ElseNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ElseNode* ElseNode::parse(Parser& parser) {
        ElseNode* node = new ElseNode();

        assert(parser.next().type() == Token::Type::KeywordElse);
        parser.parseNewline();

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

    void ElseNode::codeGen(CSourceContext& context) {
        assert(0 && "ElseNode cannot be codegen'ed to C directly");
    }
}
