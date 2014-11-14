#include "AbortStatementNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/Scope.h"

#include <assert.h>

namespace Three {
    AbortStatementNode* AbortStatementNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordAbort));

        AbortStatementNode* node = new AbortStatementNode();

        node->setStatement(true);
        node->_transactionName = parser.context()->scope()->currentScopedName("tx");
        assert(node->_transactionName.size() > 0 && "Message: abort must appear inside a transaction");

        return node;
    }

    std::string AbortStatementNode::nodeName() const {
        return "Abort";
    }

    std::string AbortStatementNode::name() const {
        return "AbortStatement";
    }

    void AbortStatementNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string AbortStatementNode::transactionName() const {
        return _transactionName;
    }
}
