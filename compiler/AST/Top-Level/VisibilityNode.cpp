#include "VisibilityNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    VisibilityNode* VisibilityNode::parse(Parser& parser) {
        VisibilityNode* node = new VisibilityNode();

        switch (parser.peek().type()) {
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

        parser.next();
        parser.parseNewline();
        parser.context()->setVisibility(node->type());

        return node;
    }

    std::string VisibilityNode::name() const {
        return "Visibility";
    }

    TranslationUnit::Visibility VisibilityNode::type() const {
        return _type;
    }

    void VisibilityNode::codeGenCSource(CSourceContext& context) {
    }
}
