#include "NamespaceNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/NewScope.h"

#include <assert.h>

namespace Three {
    NamespaceNode* NamespaceNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordNamespace));

        NamespaceNode* node = new NamespaceNode();

        node->_name = parser.parseMultiPartIdentifier();

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: new line expected after namespace definition");
        }

        parser.context()->pushScope();
        parser.context()->scope()->namespaceString = node->name();

        parser.helper()->parseUntilEnd([&] () {
            parser.helper()->parseNewlines();
            node->addChild(parser.parseTopLevelNode());
        });

        parser.context()->popScope();

        return node;
    }

    std::string NamespaceNode::nodeName() const {
        return "Namespace";
    }

    std::string NamespaceNode::name() const {
        return _name;
    }
}
