#include "AlignofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    AlignofNode* AlignofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionAlignof));

        AlignofNode* node = new AlignofNode();

        bool success = parser.parseParentheses([&] () {
            if (parser.isAtType()) {
                node->dataTypeArgument = parser.parseType();
            } else {
                node->addChild(parser.parseExpression());
            }
        });

        if (!success) {
            assert(0 && "Message: Alignof requires open and close parens");
        }

        return node;
    }

    std::string AlignofNode::nodeName() const {
        return "Alignof Metafunction";
    }

    std::string AlignofNode::name() const {
        return "Alignof";
    }

    void AlignofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
