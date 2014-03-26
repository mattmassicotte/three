#include "NextargNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    NextargNode* NextargNode::parse(Parser& parser) {
        NextargNode* node = new NextargNode();

        assert(parser.next().type() == Token::Type::MetafunctionNextarg);
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->_typeArgument = parser.parseType();

        assert(parser.next().type() == Token::Type::PunctuationComma);

        // Really, this has to be a variable reference
        node->addChild(parser.parseSecondaryExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string NextargNode::name() const {
        return "Nextarg";
    }

    void NextargNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    TypeReference NextargNode::argument() const {
        return _typeArgument;
    }
}
