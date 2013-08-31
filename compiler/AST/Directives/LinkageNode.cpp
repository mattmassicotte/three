#include "LinkageNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    LinkageNode* LinkageNode::parse(Parser& parser) {
        LinkageNode* node = new LinkageNode();

        assert(parser.next().str() == "linkage");
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);
        
        assert(parser.peek().type() == Token::Type::String);
        node->_libraryName = parser.next().str();

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        parser.parseNewline();

        return node;
    }

    std::string LinkageNode::name() const {
        return "Linkage";
    }

    std::string LinkageNode::libraryName() const {
        return _libraryName;
    }

    void LinkageNode::codeGenCSource(CSourceContext& context) {
    }
}
