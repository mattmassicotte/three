#include "ContainerofNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    ContainerofNode* ContainerofNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::MetafunctionContainerof));

        ContainerofNode* node = new ContainerofNode();

        bool success = parser.parseParentheses([&] () {
            node->addChild(parser.parseExpressionElementWithoutTailingOperators());

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Message: containerof expected a comma after the first argument");
            }

            if (!parser.isAtType()) {
                assert(0 && "Message: first argument to containerof must be a type");
            }

            node->dataTypeArgument = parser.parseType();

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Message: containerof expected a comma after the type");
            }

            if (parser.helper()->peek().type() != Token::Type::Identifier) {
                assert(0 && "Message: containerof second argument should be a member name");
            }

            node->memberName = parser.helper()->nextStr();
        });

        if (!success) {
            assert(0 && "Message: containerof requires open and close parens");
        }

        return node;
    }

    std::string ContainerofNode::nodeName() const {
        return "Containerof Metafunction";
    }

    std::string ContainerofNode::name() const {
        return "Containerof";
    }

    void ContainerofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
