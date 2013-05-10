#include "IntegerLiteralNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    IntegerLiteralNode* IntegerLiteralNode::parse(Parser& parser) {
        IntegerLiteralNode* node = new IntegerLiteralNode();

        assert(parser.peek().type() == Token::Type::NumericLiteral);

        parser.next();

        node->setValue(42);

        return node;
    }

    std::string IntegerLiteralNode::name() const {
        return "IntegerLiteral";
    }

    void IntegerLiteralNode::setValue(uint64_t v) {
        _value = v;
    }

    uint64_t IntegerLiteralNode::value() const {
        return _value;
    }

    void IntegerLiteralNode::renderCCode(std::stringstream& stream) {
        stream << this->value();
    }
}
