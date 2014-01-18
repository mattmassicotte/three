#include "ElseNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ElseNode* ElseNode::parse(Parser& parser) {
        ElseNode* node = new ElseNode();

        assert(parser.next().type() == Token::Type::KeywordElse);
        parser.parseNewline();

        // parse the body
        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    std::string ElseNode::name() const {
        return "Else";
    }

    void ElseNode::codeGen(CSourceContext& context) {
        assert(0 && "ElseNode cannot be codegen'ed to C directly");
    }
}
