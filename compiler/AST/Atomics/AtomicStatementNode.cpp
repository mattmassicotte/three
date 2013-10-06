#include "AtomicStatementNode.h"
#include "../../Parser.h"
#include "AtomicExpressionNode.h"

#include <assert.h>

namespace Three {
    ASTNode* AtomicStatementNode::parse(Parser& parser) {
        AtomicStatementNode* node = new AtomicStatementNode();

        assert(parser.next().type() == Token::Type::KeywordAtomic);

        if (parser.nextIf(":")) {
            assert(parser.next().str() == "fallback");
            assert(0 && "atomic:fallback not supported");
        }

        parser.parseNewline();

        parser.pushScope(new Scope("atomic"));
        parser.currentScope()->setTransactional(true);

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        parser.popScope();

        return node;
    }

    std::string AtomicStatementNode::name() const {
        return "AtomicStatement";
    }

    void AtomicStatementNode::codeGenCSource(CSourceContext& context) {
        context.addHeader("stdbool.h");
        context.addHeader("three/runtime/transactional_memory.h");

        context << "if (three_transaction_begin(";
        context << "NULL";
        context.current()->printLineAndIndent(")) {");

        this->codeGenCSourceForChildren(context);

        context << "three_transaction_end(";
        context << "NULL";
        context.current()->printLine(");");

        context.current()->outdentAndPrintLine("}");
    }
}
