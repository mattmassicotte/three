#include "TypeofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    TypeofNode* TypeofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionTypeof));

        TypeofNode* node = new TypeofNode();

        bool success = parser.parseParentheses([&] () {
            if (parser.isAtType()) {
                node->dataTypeArgument = parser.parseType();
            } else {
                node->addChild(parser.parseExpression());
            }
        });

        if (!success) {
            assert(0 && "Message: Typeof requires open and close parens");
        }

        return node;
    }

    std::string TypeofNode::nodeName() const {
        return "Typeof Metafunction";
    }

    std::string TypeofNode::name() const {
        return "Typeof";
    }

    void TypeofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
