#include "NextargNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    NextargNode* NextargNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionNextarg));

        NextargNode* node = new NextargNode();

        bool success = parser.parseParentheses([&] () {
            node->dataTypeArgument = parser.parseType();

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Message: nextarg type argument should be followed by a comma");
            }

            node->addChild(parser.parseExpression());
        });

        if (!success) {
            assert(0 && "Message: nextarg requires open and close parens");
        }

        return node;
    }

    std::string NextargNode::nodeName() const {
        return "Nextarg Metafunction";
    }

    std::string NextargNode::name() const {
        return "Nextarg";
    }

    void NextargNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
