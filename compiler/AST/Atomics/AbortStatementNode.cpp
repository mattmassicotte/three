#include "AbortStatementNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    AbortStatementNode* AbortStatementNode::parse(Parser& parser) {
        AbortStatementNode* node = new AbortStatementNode();

        assert(parser.next().type() == Token::Type::KeywordAbort);

        node->setStatement(true);

        return node;
    }

    std::string AbortStatementNode::name() const {
        return "AbortStatement";
    }

    void AbortStatementNode::codeGen(CSourceContext& context) {
        context.declarations()->addHeader(false, "three/runtime/transactional_memory.h");

        context << "three_transaction_abort(&";
        // TODO: transaction name will need to tracked somehow
        context << "tx1";
        context << ")";
    }
}
