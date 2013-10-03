#include "BreakNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    BreakNode* BreakNode::parse(Parser& parser) {
        BreakNode* node = new BreakNode();

        assert(parser.next().type() == Token::Type::KeywordBreak);

        return node;
    }

    std::string BreakNode::name() const {
        return "Break";
    }

    void BreakNode::codeGenCSource(CSourceContext& context) {
        context.current()->printLine("break;");
    }
}
