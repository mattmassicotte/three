#include "CardinalityofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    CardinalityofNode* CardinalityofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionCardinalityof));

        CardinalityofNode* node = new CardinalityofNode();

        bool success = parser.parseParentheses([&] () {
            node->addChild(parser.parseExpression());
        });

        if (!success) {
            assert(0 && "Message: Cardinalityof requires open and close parens");
        }

        return node;
    }

    std::string CardinalityofNode::nodeName() const {
        return "Cardinalityof Metafunction";
    }

    std::string CardinalityofNode::name() const {
        return "Cardinalityof";
    }

    void CardinalityofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
