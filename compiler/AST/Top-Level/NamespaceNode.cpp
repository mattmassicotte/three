#include "NamespaceNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    NamespaceNode* NamespaceNode::parse(Parser& parser) {
        NamespaceNode* node = new NamespaceNode();

        assert(parser.next().type() == Token::Type::KeywordNamespace);

        node->_name = parser.parseQualifiedName();
        assert(node->_name.length() > 0);

        parser.parseNewline();
        parser.context()->setNamespace(node->_name);

        return node;
    }

    std::string NamespaceNode::name() const {
        return "Namespace";
    }

    std::string NamespaceNode::namespaceName() const {
        return _name;
    }

    void NamespaceNode::codeGenCSource(CSourceContext& context) {
    }
}
