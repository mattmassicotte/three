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

        node->_lockFunctionNode = nullptr;
        node->_unlockFunctionNode = nullptr;
        node->_lockContextNode = nullptr;

        if (parser.helper()->nextIf(Token::Type::PunctuationColon)) {
            if (!parser.helper()->nextIf("fallback")) {
                std::cout << parser.helper()->peek().str() << std::endl;
                assert(0 && "Unhandled: unrecognized atomic statement specifier");
            }

            if (!parser.helper()->nextIf(Token::Type::PunctuationOpenParen)) {
                std::cout << parser.helper()->peek().str() << std::endl;
                assert(0 && "Unhandled: expected open paren after fallback");
            }

            node->_lockFunctionNode = parser.parseExpressionElement();

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Unhandled: expected comma after lock expression");
            }

            node->_unlockFunctionNode = parser.parseExpressionElement();

            if (!parser.helper()->nextIf(Token::Type::PunctuationComma)) {
                assert(0 && "Unhandled: expected comma after lock expression");
            }

            node->_lockContextNode = parser.parseExpressionElement();

            if (!parser.helper()->nextIf(Token::Type::PunctuationCloseParen)) {
                std::cout << parser.helper()->peek().str() << std::endl;
                assert(0 && "Unhandled: expected open paren after fallback");
            }
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

    ASTNode* AtomicStatementNode::lockFunction() const {
        return _lockFunctionNode;
    }

    ASTNode* AtomicStatementNode::unlockFunction() const {
        return _unlockFunctionNode;
    }

    ASTNode* AtomicStatementNode::lockContext() const {
        return _lockContextNode;
    }
}
