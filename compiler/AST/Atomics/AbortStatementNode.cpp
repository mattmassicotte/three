#include "AbortStatementNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    AbortStatementNode* AbortStatementNode::parse(Parser& parser) {
        AbortStatementNode* node = new AbortStatementNode();

        assert(parser.next().type() == Token::Type::KeywordAbort);

        node->setStatement(true);

        return node;
    }

    std::string AbortStatementNode::name() const {
        return "AbortStatement";
    }

    void AbortStatementNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("three/runtime/transactional_memory.h");

        context << "three_transaction_abort();";
    }
}
