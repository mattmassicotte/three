#include "BreakNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    BreakNode* BreakNode::parse(Parser& parser) {
        BreakNode* node = new BreakNode();

        assert(parser.next().type() == Token::Type::KeywordBreak);
        node->setStatement(true);

        return node;
    }

    std::string BreakNode::name() const {
        return "Break";
    }

    void BreakNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void BreakNode::codeGen(CSourceContext& context) {
        context << "break";
    }
}
