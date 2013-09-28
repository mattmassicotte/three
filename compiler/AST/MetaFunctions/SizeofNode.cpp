#include "SizeofNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    SizeofNode* SizeofNode::parse(Parser& parser) {
        SizeofNode* node = new SizeofNode();

        assert(parser.next().type() == Token::Type::KeywordSizeof);
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->_typeArgument = parser.parseType();

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string SizeofNode::name() const {
        return "MetaFunction";
    }

    void SizeofNode::codeGenCSource(CSourceContext& context) {
        context << "sizeof(";

        _typeArgument.codeGenCSource(context.current(), "");

        context << ")";
    }
}