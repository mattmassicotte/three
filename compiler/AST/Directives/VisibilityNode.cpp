#include "VisibilityNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    VisibilityNode* VisibilityNode::parse(NewParser& parser) {
        VisibilityNode* node = new VisibilityNode();

        switch (parser.helper()->next().type()) {
            case Token::Type::KeywordPublic:
                node->_type = TranslationUnit::Visibility::External;
                break;
            case Token::Type::KeywordPrivate:
                node->_type = TranslationUnit::Visibility::None;
                break;
            default:
                assert(0 && "Unexpected value in VisibilityNode::parse");
                break;
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: a visibility directive must be followed by a newline");
        }

        parser.context()->setVisibility(node->type());

        return node;
    }

    std::string VisibilityNode::nodeName() const {
        return "Visibility";
    }

    std::string VisibilityNode::name() const {
        return "Visibility";
    }

    TranslationUnit::Visibility VisibilityNode::type() const {
        return _type;
    }
}
