#include "NullLiteralNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    NullLiteralNode* NullLiteralNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::LiteralNull));

        NullLiteralNode* node = new NullLiteralNode();

        DataType type = DataType(DataType::Kind::Pointer);
        type.addSubtype(parser.context()->typeKindWithName("Void"));

        node->setDataType(type);

        return node;
    }

    std::string NullLiteralNode::nodeName() const {
        return "Null Literal";
    }

    std::string NullLiteralNode::name() const {
        return "NullLiteral";
    }

    void NullLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
