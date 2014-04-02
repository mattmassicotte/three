#include "NamespaceNode.h"
#include "compiler/Parser/NewParser.h"
#include "compiler/constructs/NewScope.h"

#include <assert.h>

namespace Three {
    NamespaceNode* NamespaceNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordNamespace));

        NamespaceNode* node = new NamespaceNode();

        assert(parser.helper()->peek().type() == Token::Type::Identifier);
        if (!parser.isAtIdentifierAvailableForDefinition()) {
            assert(0 && "Message: namespace name isn't avaiable for definition");
        }

        node->_name = parser.helper()->nextStr();

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: new line expected after namespace definition");
        }

        parser.context()->pushScope();
        parser.context()->scope()->namespaceString = node->_name;

        parser.helper()->parseUntilEnd([&] () {
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
