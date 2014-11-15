#include "NameofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    NameofNode* NameofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionNameof));

        NameofNode* node = new NameofNode();

        bool success = parser.parseParentheses([&] () {
            if (parser.isAtType()) {
                node->dataTypeArgument = parser.parseType();
            } else {
                node->addChild(parser.parseExpression());
            }
        });

        if (!success) {
            assert(0 && "Message: Nameof requires open and close parens");
        }

        return node;
    }

    std::string NameofNode::nodeName() const {
        return "Nameof Metafunction";
    }

    std::string NameofNode::name() const {
        return "Nameof";
    }

    void NameofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
