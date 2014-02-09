#include "NullLiteralNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    NullLiteralNode* NullLiteralNode::parse(Parser& parser) {
        NullLiteralNode* node = new NullLiteralNode();

        assert(parser.next().type() == Token::Type::LiteralNull);

        return node;
    }

    std::string NullLiteralNode::name() const {
        return "NullLiteral";
    }

    void NullLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void NullLiteralNode::codeGen(CSourceContext& context) {
        context << "NULL";
    }
}
