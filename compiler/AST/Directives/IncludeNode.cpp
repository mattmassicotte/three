#include "IncludeNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    IncludeNode* IncludeNode::parse(Parser& parser) {
        IncludeNode* node = new IncludeNode();

        assert(parser.next().str() == "include");
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        assert(parser.peek().type() == Token::Type::String);
        node->_headerName = parser.next().str();

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        parser.parseNewline();

        parser.currentModule()->addCIncludePath(node->_headerName);

        return node;
    }

    std::string IncludeNode::name() const {
        return "Include";
    }

    std::string IncludeNode::headerName() const {
        return _headerName;
    }

    void IncludeNode::codeGenCSource(CSourceContext& context) {
    }
}
