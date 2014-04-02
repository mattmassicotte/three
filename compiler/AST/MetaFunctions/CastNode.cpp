#include "CastNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    CastNode* CastNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionCast));

        CastNode* node = new CastNode();

        bool success = parser.parseParentheses([&] () {
            node->dataTypeArgument = parser.parseType();

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Message: cast type argument should be followed by a comma");
            }

            node->addChild(parser.parseExpression());
        });

        if (!success) {
            assert(0 && "Message: cast requires open and close parens");
        }

        return node;
    }

    std::string CastNode::nodeName() const {
        return "Cast Metafunction";
    }

    std::string CastNode::name() const {
        return "Cast";
    }

    void CastNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
