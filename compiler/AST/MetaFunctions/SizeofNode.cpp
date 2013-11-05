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
        return "MetaFunction";
    }

    void SizeofNode::codeGen(CSourceContext& context) {
        context << "sizeof(";

        _typeArgument.codeGen(context);

        context << ")";
    }
}
