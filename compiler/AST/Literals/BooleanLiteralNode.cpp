#include "BooleanLiteralNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    BooleanLiteralNode* BooleanLiteralNode::parse(Parser& parser) {
        BooleanLiteralNode* node = new BooleanLiteralNode();

        switch (parser.next().type()) {
            case Token::Type::TrueLiteral:
                node->_value = true;
                break;
            case Token::Type::FalseLiteral:
                node->_value = false;
                break;
            default:
                assert(0 && "Expected boolean literal");
                break;
        }

        return node;
    }

    std::string BooleanLiteralNode::name() const {
        return "BooleanLiteral";
    }

    bool BooleanLiteralNode::value() const {
        return _value;
    }

    void BooleanLiteralNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("stdbool.h");

        context << (this->value() ? "true" : "false");
    }
}
