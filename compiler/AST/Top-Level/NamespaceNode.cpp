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

        parser.pushScope(new Scope(node->_name));
        parser.currentScope()->setNamespace(node->_name);

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseTopLevelNode());
        });

        parser.popScope();
        parser.parseNewline();

        return node;
    }

    std::string NamespaceNode::name() const {
        return "Namespace";
    }

    std::string NamespaceNode::namespaceName() const {
        return _name;
    }

    void NamespaceNode::codeGenCSource(CSourceContext& context) {
        this->codeGenCSourceForChildren(context);
    }
}
