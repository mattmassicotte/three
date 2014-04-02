#include "SizeofNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    SizeofNode* SizeofNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionSizeOf));

        SizeofNode* node = new SizeofNode();

        bool success = parser.parseParentheses([&] () {
            if (parser.isAtType()) {
                node->dataTypeArgument = parser.parseType();
            } else {
                node->addChild(parser.parseExpression());
            }
        });

        if (!success) {
            assert(0 && "Message: sizeof requires open and close parens");
        }

        return node;
    }

    std::string SizeofNode::nodeName() const {
        return "Sizeof Metafunction";
    }

    std::string SizeofNode::name() const {
        return "SizeOf";
    }

    void SizeofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
