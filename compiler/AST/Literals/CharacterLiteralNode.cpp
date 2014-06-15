#include "CharacterLiteralNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    CharacterLiteralNode* CharacterLiteralNode::parse(Parser& parser) {
        CharacterLiteralNode* node = new CharacterLiteralNode();

        assert(parser.helper()->peek().type() == Token::Type::LiteralCharacter);
        node->setValue(parser.helper()->next().strTrimmingFirstAndLast());

        NewDataType charType = parser.context()->typeKindWithName("Char");

        // here we have to check for type specifiers
        if (parser.helper()->nextIf(Token::Type::PunctuationColon)) {
            charType.setCharacterEncoding(parser.parseCharacterEncodingSpecifier());
        }

        node->setDataType(charType);

        return node;
    }

    std::string CharacterLiteralNode::nodeName() const {
        return "Character Literal";
    }

    void CharacterLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void CharacterLiteralNode::setValue(const std::string& value) {
        _value = value;
    }

    std::string CharacterLiteralNode::value() const {
        return _value;
    }
}
