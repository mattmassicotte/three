#include "IncludeNode.h"
#include "../../Parser.h"
#include "../../CSourceIndexer.h"

#include <assert.h>

namespace Three {
    IncludeNode* IncludeNode::parse(Parser& parser) {
        IncludeNode* node = new IncludeNode();

        assert(parser.next().str() == "include");

        assert(parser.peek().type() == Token::Type::String);
        node->_headerName = parser.next().str();

        parser.parseNewline();

        parser.currentModule()->addCIncludePath(node->_headerName);

        CSourceIndexer index;

        index.indexFileAtPath(node->_headerName);

        parser.currentModule()->addModule(node->_headerName, index.module());

        return node;
    }

    std::string IncludeNode::name() const {
        return "Include";
    }

    std::string IncludeNode::headerName() const {
        return _headerName;
    }

    void IncludeNode::codeGenCSource(CSourceContext& context) {
        context.addHeader(this->_headerName);
    }
}
