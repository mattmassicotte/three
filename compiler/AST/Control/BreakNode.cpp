#include "BreakNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    BreakNode* BreakNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordBreak));

        BreakNode* node = new BreakNode();

        node->setStatement(true);

        return node;
    }

    std::string BreakNode::nodeName() const {
        return "Break";
    }

    std::string BreakNode::name() const {
        return "Break";
    }

    void BreakNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
