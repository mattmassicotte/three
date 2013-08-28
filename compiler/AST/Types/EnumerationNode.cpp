#include "EnumerationNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    EnumerationNode* EnumerationNode::parse(Parser& parser) {
        EnumerationNode* node = new EnumerationNode();

        assert(parser.next().type() == Token::Type::KeywordEnumeration);

        assert(parser.peek().type() == Token::Type::Identifier);
        node->_name = parser.next().str();
        parser.parseNewline();

        while (parser.peek().type() != Token::Type::KeywordEnd) {
            assert(parser.peek().type() == Token::Type::Identifier);
            node->_identifiers.push_back(parser.next().str());
            parser.parseNewline();
        }

        assert(parser.next().type() == Token::Type::KeywordEnd);
        parser.parseNewline();

        return node;
    }

    std::string EnumerationNode::name() const {
        return "Enumeration";
    }

    std::string EnumerationNode::structureName() const {
        return _name;
    }

    void EnumerationNode::codeGenCSource(CSourceContext& context) {
        context.setCurrent(CSourceContext::Section::Declarations);

        context.current()->printLineAndIndent("typedef enum {");

        for (uint32_t i = 0; i < _identifiers.size(); ++i) {
            context.current()->printLine(_identifiers.at(i));
        }

        context.current()->outdentAndPrintLine("} " + this->structureName() + ";");

        context.setCurrent(CSourceContext::Section::Body);
    }
}
