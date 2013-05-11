#include "IntegerLiteralNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    IntegerLiteralNode* IntegerLiteralNode::parse(Parser& parser) {
        IntegerLiteralNode* node = new IntegerLiteralNode();

        assert(parser.peek().type() == Token::Type::NumericLiteral);

        node->setValue(integerValue(parser.next().str()));

        return node;
    }

    uint64_t IntegerLiteralNode::integerValue(const std::string& string) {
        const char* cString = string.c_str();

        // might have a zero-encoded string, but check for length, because
        // it could just be zero
        if ((cString[0] == '0') && (string.length() > 1)) {
            if (cString[1] == 'x') {
                return strtol(cString, NULL, 16);
            }

            assert(0 && "Unhandled integer value type");
        }

        return strtol(cString, NULL, 10);
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

    void IntegerLiteralNode::renderCCode(std::stringstream& stream, uint32_t indentation) {
        stream << this->value();
    }
}
