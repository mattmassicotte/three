#include "IntegerLiteralNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>
#include <sstream>

namespace Three {
    IntegerLiteralNode* IntegerLiteralNode::parse(NewParser& parser) {
        switch (parser.helper()->peek().type()) {
            case Token::Type::LiteralInteger:
            case Token::Type::LiteralBinary:
            case Token::Type::LiteralHex:
                break;
            default:
                assert(0 && "Expected integer literal token");
                break;
        }

        IntegerLiteralNode* node = new IntegerLiteralNode();

        node->setValue(integerValue(parser.helper()->nextStr()));

        // here we have to check for type specifiers, but we could be at a range
        if (parser.helper()->peek().type() == Token::Type::PunctuationColon) {
            if (parser.helper()->peek(2).type() == Token::Type::Identifier) {
                assert(parser.helper()->nextIf(Token::Type::PunctuationColon));
                node->setDataType(parser.parseTypeWithoutAnnotations());
                return node;
            }
        }

        node->setDataType(parser.context()->typeKindWithName("Int"));

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

    std::string IntegerLiteralNode::nodeName() const {
        return "Integer Literal";
    }

    std::string IntegerLiteralNode::name() const {
        return "IntegerLiteral";
    }

    void IntegerLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void IntegerLiteralNode::setValue(uint64_t v) {
        _value = v;
    }

    uint64_t IntegerLiteralNode::value() const {
        return _value;
    }
}
