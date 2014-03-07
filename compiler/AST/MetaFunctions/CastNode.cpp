#include "CastNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    CastNode* CastNode::parse(Parser& parser) {
        CastNode* node = new CastNode();

        assert(parser.next().type() == Token::Type::MetafunctionCast);
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->_typeArgument = parser.parseType();

        assert(parser.next().type() == Token::Type::PunctuationComma);

        // Cannot have tailing conditionals, so must be secondary
        node->addChild(parser.parseSecondaryExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string CastNode::name() const {
        return "Cast";
    }

    void CastNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    TypeReference CastNode::argument() const {
        return _typeArgument;
    }
}
