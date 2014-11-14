#include "AtomicStatementNode.h"
#include "AtomicExpressionNode.h"
#include "../Control/ElseNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/constructs/Scope.h"

#include <assert.h>

namespace Three {
    ASTNode* AtomicStatementNode::parse(Parser& parser) {
        if (AtomicNode::isAtAtomicExpression(parser)) {
            return nullptr;
        }

        assert(parser.helper()->nextIf(Token::Type::KeywordAtomic));

        AtomicStatementNode* node = new AtomicStatementNode();

        if (parser.helper()->nextIf(":")) {
            assert(0 && "Unhandled: atomic statement specifiers aren't built yet :(");
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: new line after atomic expected");
        }

        node->_transactionName = parser.context()->scope()->scopedName("tx");

        if (!parser.parseBodyWithElse(node, &node->_elseNode)) {
            assert(0 && "Message: Unable to parse atomic statement body");
        }

        return node;
    }

    std::string AtomicStatementNode::nodeName() const {
        return "Atomic Statement";
    }

    std::string AtomicStatementNode::name() const {
        return "AtomicStatement";
    }

    void AtomicStatementNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string AtomicStatementNode::transactionName() const {
        return _transactionName;
    }

    ASTNode* AtomicStatementNode::elseClause() const {
        return _elseNode;
    }
}
