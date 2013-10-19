#include "IntegerLiteralNode.h"
#include "../../Parser.h"

#include <assert.h>
#include <sstream>

namespace Three {
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
            assert(string.length() > 2 && "A prefixed integer must be at least 3 characters");

            if (cString[1] == 'x') {
                return strtol(cString, NULL, 16);
            } else if (cString[1] == 'b') {
                // we need to remove the "0b" prefix
                return strtol(&(cString[2]), NULL, 2);
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

    void IntegerLiteralNode::codeGen(CSourceContext& context) {
        std::stringstream s;

        // TODO: this is kinda stupid just to convert into a string...
        s << this->value();

        context << s.str();
    }
}
