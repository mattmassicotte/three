#include "BooleanLiteralNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    BooleanLiteralNode* BooleanLiteralNode::parse(NewParser& parser) {
        BooleanLiteralNode* node = new BooleanLiteralNode();

        switch (parser.helper()->next().type()) {
            case Token::Type::LiteralTrue:
                node->_value = true;
                break;
            case Token::Type::LiteralFalse:
                node->_value = false;
                break;
            default:
                assert(0 && "Message: Expected boolean literal");
                break;
        }

        node->setDataType(parser.context()->typeKindWithName("Bool"));

        return node;
    }

    std::string BooleanLiteralNode::nodeName() const {
        return "Boolean Literal";
    }

    std::string BooleanLiteralNode::name() const {
        return "BooleanLiteral";
    }

    void BooleanLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    bool BooleanLiteralNode::value() const {
        return _value;
    }
}
