#include "NamespaceNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/NewScope.h"

#include <assert.h>
#include <sstream>

namespace Three {
    NamespaceNode* NamespaceNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordNamespace));

        NamespaceNode* node = new NamespaceNode();

        node->_name = parser.parseMultiPartIdentifierComponents();

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: new line expected after namespace definition");
        }

        parser.context()->pushScope();
        parser.context()->scope()->setNamespace(node->_name);

        bool parsedSuccessfully = true;

        // TODO: the success conditions need to be improved here
        parser.helper()->parseUntilEnd([&] () {
            bool success = false;

            if (!node->addChild(parser.parseTopLevelNode(&success))) {
                parsedSuccessfully = success;
                return true;
            }

            return false;
        });

        if (!parsedSuccessfully) {
            delete node;

            return nullptr;
        }

        parser.context()->popScope();

        return node;
    }

    std::string NamespaceNode::nodeName() const {
        return "Namespace";
    }

    std::string NamespaceNode::name() const {
        return _name.to_s();
    }

    QualifiedName NamespaceNode::qualifiedName() const {
        return _name;
    }
}
