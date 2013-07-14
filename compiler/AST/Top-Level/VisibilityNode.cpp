#include "VisibilityNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    VisibilityNode* VisibilityNode::parse(Parser& parser) {
        VisibilityNode* node = new VisibilityNode();

        assert(parser.next().str() == "public");
        parser.parseNewline();

        node->_type = Language::TranslationUnit::Visibility::External;

        return node;
    }

    std::string VisibilityNode::name() const {
        return "Visibility";
    }

    Language::TranslationUnit::Visibility VisibilityNode::type() const {
        return _type;
    }

    void VisibilityNode::codeGenCSource(CSourceContext& context) {
    }
}
