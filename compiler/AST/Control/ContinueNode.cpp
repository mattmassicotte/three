#include "ContinueNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    ContinueNode* ContinueNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordContinue));

        ContinueNode* node = new ContinueNode();

        node->setStatement(true);

        return node;
    }

    std::string ContinueNode::nodeName() const {
        return "Continue";
    }

    std::string ContinueNode::name() const {
        return "Continue";
    }

    void ContinueNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
