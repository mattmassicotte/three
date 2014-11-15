#include "OffsetofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    OffsetofNode* OffsetofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionOffsetof));

        OffsetofNode* node = new OffsetofNode();

        bool success = parser.parseParentheses([&] () {
            if (!parser.isAtType()) {
                assert(0 && "Message: first argument to offsetof must be a type");
            }

            node->dataTypeArgument = parser.parseType();

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Message: offsetof expected a comma after the type");
            }

            if (parser.helper()->peek().type() != Token::Type::Identifier) {
                assert(0 && "Message: offsetof second argument should be a member name");
            }

            node->memberName = parser.helper()->nextStr();
        });

        if (!success) {
            assert(0 && "Message: Offsetof requires open and close parens");
        }

        return node;
    }

    std::string OffsetofNode::nodeName() const {
        return "Offsetof Metafunction";
    }

    std::string OffsetofNode::name() const {
        return "Offsetof";
    }

    void OffsetofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
