#include "BarrierNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    BarrierNode* BarrierNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordBarrier));

        BarrierNode* node = new BarrierNode();

        AtomicNode::parseOrdering(parser, node);

        return node;
    }

    std::string BarrierNode::name() const {
        return "Barrier";
    }

    void BarrierNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
