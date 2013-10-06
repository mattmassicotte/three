#include "ContinueNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ContinueNode* ContinueNode::parse(Parser& parser) {
        ContinueNode* node = new ContinueNode();

        assert(parser.next().type() == Token::Type::KeywordContinue);

        node->setStatement(true);

        return node;
    }

    std::string ContinueNode::name() const {
        return "Continue";
    }

    void ContinueNode::codeGen(CSourceContext& context) {
        context << "continue";
    }
}
