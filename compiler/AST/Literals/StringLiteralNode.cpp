#include "StringLiteralNode.h"
#include "../../Parser.h"

#include <assert.h>
#include <sstream>

namespace Three {
    StringLiteralNode* StringLiteralNode::parse(Parser& parser) {
        StringLiteralNode* node;

        node = new StringLiteralNode();

        assert(parser.peek().type() == Token::Type::LiteralString);
        node->setStringValue(parser.next().str());

        return node;
    }

    std::string StringLiteralNode::name() const {
        return "StringLiteral";
    }

    std::string StringLiteralNode::str() const {
        std::stringstream s;

        s << this->name() << ": " << this->stringValue();

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

    void StringLiteralNode::codeGen(CSourceContext& context) {
        context << "\"" << this->stringValue() << "\"";
    }
}
