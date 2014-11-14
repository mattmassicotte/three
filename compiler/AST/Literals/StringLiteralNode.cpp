#include "StringLiteralNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>
#include <sstream>

namespace Three {
    StringLiteralNode* StringLiteralNode::parse(Parser& parser) {
        StringLiteralNode* node = new StringLiteralNode();

        assert(parser.helper()->peek().type() == Token::Type::LiteralString);
        node->setStringValue(parser.helper()->next().strTrimmingFirstAndLast());

        DataType charType = parser.context()->typeKindWithName("Char");

        // here we have to check for type specifiers
        if (parser.helper()->nextIf(Token::Type::PunctuationColon)) {
            charType.setCharacterEncoding(parser.parseCharacterEncodingSpecifier());
        }

        DataType type = DataType(DataType::Kind::Pointer);
        type.addSubtype(charType);

        node->setDataType(type);

        return node;
    }

    std::string StringLiteralNode::nodeName() const {
        return "String Literal";
    }

    std::string StringLiteralNode::name() const {
        return "StringLiteral";
    }

    std::string StringLiteralNode::str() const {
        std::stringstream s;

        s << this->nodeName() << ": " << this->stringValue();

        return s.str();
    }

    void StringLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void StringLiteralNode::setStringValue(const std::string& name) {
        _stringValue = name;
    }

    std::string StringLiteralNode::stringValue() const {
        return _stringValue;
    }
}
