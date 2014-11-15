#include "ABIofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    ABIofNode* ABIofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionABIof));

        ABIofNode* node = new ABIofNode();

        bool success = parser.parseParentheses([&] () {
            node->addChild(parser.parseExpressionElement());
        });

        if (!success) {
            assert(0 && "Message: Alignof requires open and close parens");
        }

        return node;
    }

    std::string ABIofNode::nodeName() const {
        return "ABIof Metafunction";
    }

    std::string ABIofNode::name() const {
        return "ABIof";
    }

    void ABIofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
