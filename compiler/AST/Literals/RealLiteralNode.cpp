#include "RealLiteralNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    RealLiteralNode* RealLiteralNode::parse(NewParser& parser) {
        RealLiteralNode* node = new RealLiteralNode();

        if (parser.helper()->peek().type() == Token::Type::LiteralReal) {
            node->_value = strtod(parser.helper()->next().str().c_str(), NULL);
        }

        // here we have to check for type specifiers
        if (parser.helper()->nextIf(Token::Type::PunctuationColon)) {
            node->setDataType(parser.parseTypeWithoutAnnotations());
        } else {
            node->setDataType(parser.context()->typeKindWithName("Float"));
        }

        return node;
    }

    std::string RealLiteralNode::nodeName() const {
        return "Real Literal";
    }

    void RealLiteralNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void RealLiteralNode::setValue(double v) {
        _value = v;
    }

    double RealLiteralNode::value() const {
        return _value;
    }
}
