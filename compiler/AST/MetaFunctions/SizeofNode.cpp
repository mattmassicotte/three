#include "SizeofNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    SizeofNode* SizeofNode::parse(Parser& parser) {
        SizeofNode* node = new SizeofNode();

        assert(parser.next().type() == Token::Type::MetafunctionSizeOf);
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->_typeArgument = parser.parseType();

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string SizeofNode::name() const {
        return "SizeOf";
    }

    void SizeofNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    TypeReference SizeofNode::argument() const {
        return _typeArgument;
    }

    void SizeofNode::codeGen(CSourceContext& context) {
        context << "sizeof(";

        context << _typeArgument.codeGen();

        context << ")";
    }
}
