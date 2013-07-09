#include "NullLiteralNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    NullLiteralNode* NullLiteralNode::parse(Parser& parser) {
        NullLiteralNode* node = new NullLiteralNode();

        assert(parser.next().type() == Token::Type::NullLiteral);

        return node;
    }

    std::string NullLiteralNode::name() const {
        return "NullLiteral";
    }

    void NullLiteralNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("stddef.h");

        context << "NULL";
    }
}
