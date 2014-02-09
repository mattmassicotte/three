#include "AtomicStatementNode.h"
#include "AtomicExpressionNode.h"
#include "../../Parser.h"
#include "../Control/ElseNode.h"

#include <assert.h>

namespace Three {
    ASTNode* AtomicStatementNode::parse(Parser& parser) {
        AtomicStatementNode* node = new AtomicStatementNode();

        assert(parser.next().type() == Token::Type::KeywordAtomic);

        if (parser.nextIf(":")) {
            assert(parser.next().str() == "fallback");
            assert(0 && "atomic:fallback not supported");
        }

        // TODO: this is not correct
        node->_transactionName = "tx1";

        parser.parseNewline();

        parser.pushScope(new Scope());
        parser.currentScope()->setTransactional(true);

        parser.parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::KeywordEnd:
                    return true;
                case Token::Type::KeywordElse:
                    return true;
                default:
                    node->addChild(parser.parseStatement());
                    return false;
                }
        });

        parser.popScope();

        if (parser.peek().type() != Token::Type::KeywordElse) {
            assert(parser.next().type() == Token::Type::KeywordEnd);
            node->_elseNode = NULL;

            return node;
        }

        node->_elseNode = new ElseNode();

        parser.pushScope(new Scope());

        assert(parser.next().type() == Token::Type::KeywordElse);
        parser.parseNewline();
        parser.parseUntilEnd([&] () {
            node->_elseNode->addChild(parser.parseStatement());
        });

        parser.popScope();

        return node;
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

    void AtomicStatementNode::codeGen(CSourceContext& context) {
        context.declarations()->addHeader(false, "three/runtime/transactional_memory.h");

        this->codeGenTransactionAllocation(context);

        context << "if (three_transaction_begin(&" << this->_transactionName;
        context.current()->printLineAndIndent(")) {");

        this->codeGenChildren(context);

        context << "three_transaction_end(&" << this->_transactionName;
        context.current()->printLine(");");

        if (_elseNode) {
            context.current()->outdentAndPrintLine("} else {");
            context.current()->increaseIndentation();

            _elseNode->codeGenChildren(context);
        } else {
            context.current()->outdentAndPrintLine("} else {");

            context.declarations()->addHeader(false, "assert.h");

            context.current()->increaseIndentation();

            // assert(0 && "transaction 'tx1' failed without any fallback path");
            context << "assert(0 && \"transaction '" << this->_transactionName;
            context << "' failed without any fallback path\"";
            context.current()->printLine(");");
        }

        context.current()->outdentAndPrintLine("}");
    }

    void AtomicStatementNode::codeGenTransactionAllocation(CSourceContext& context) {
        context << "three_transaction_t ";
        context << this->_transactionName;
        context << " = THREE_MAKE_DEFAULT_TRANSACTION()";
        context.current()->printLine(";");
    }
}
