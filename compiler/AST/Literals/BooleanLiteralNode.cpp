#include "BooleanLiteralNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    BooleanLiteralNode* BooleanLiteralNode::parse(Parser& parser) {
        BooleanLiteralNode* node = new BooleanLiteralNode();

        switch (parser.next().type()) {
            case Token::Type::LiteralTrue:
                node->_value = true;
                break;
            case Token::Type::LiteralFalse:
                node->_value = false;
                break;
            default:
                assert(0 && "Expected boolean literal");
                break;
        }

        node->_type = parser.context()->refForName("Bool", 0);

        return node;
    }

    std::string BooleanLiteralNode::name() const {
        return "BooleanLiteral";
    }

    void BooleanLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    TypeReference BooleanLiteralNode::nodeType() const {
        return _type;
    }

    bool BooleanLiteralNode::value() const {
        return _value;
    }
}
