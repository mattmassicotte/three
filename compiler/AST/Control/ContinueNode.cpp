#include "ContinueNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ContinueNode* ContinueNode::parse(Parser& parser) {
        ContinueNode* node = new ContinueNode();

        assert(parser.next().type() == Token::Type::KeywordContinue);

        return node;
    }

    std::string ContinueNode::name() const {
        return "Continue";
    }

    void ContinueNode::codeGenCSource(CSourceContext& context) {
        context.current()->printLine("continue;");
    }
}
