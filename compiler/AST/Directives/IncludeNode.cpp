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
        node->_visibility = parser.context()->visibility();

        parser.parseNewline();

        parser.currentModule()->addCIncludePath(node->_headerName);

        CSourceIndexer index;

        if (!index.indexFileAtPath(node->_headerName)) {
            std::cout << "Unable to index c header '" << node->_headerName << "'" << std::endl;
            assert(0);
        }

        parser.currentModule()->addModule(node->_headerName, index.module());

        return node;
    }

    std::string IncludeNode::name() const {
        return "Include";
    }

    std::string IncludeNode::headerName() const {
        return _headerName;
    }

    void IncludeNode::codeGen(CSourceContext& context) {
        context.adjustCurrentForVisibility(this->_visibility, [&] (CSource* source) {
            source->addHeader(false, this->_headerName);
        });
    }
}
