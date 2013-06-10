#include "AtomicStatementNode.h"
#include "../../Parser.h"
#include "AtomicExpressionNode.h"

#include <assert.h>

namespace Language {
    ASTNode* AtomicStatementNode::parse(Parser& parser) {
        std::string type;

        AtomicStatementNode* node = new AtomicStatementNode();

        assert(parser.next().type() == Token::Type::KeywordAtomic);

        if (parser.nextIf(":")) {
            type = parser.next().str();
        } else {
            type = "ordered";
        }

        if (parser.peek().type() == Token::Type::PunctuationOpenParen) {
            assert(type != "strict");
            return AtomicExpressionNode::parse(parser, type);
        }

        parser.parseNewline(true);

        while (true) {
            if (parser.peek().type() == Token::Type::KeywordEnd) {
                parser.next();
                break;
            }

            if (parser.peek().type() == Token::Type::KeywordElse) {
                node->_elseNode = ElseNode::parse(parser);
                break;
            }

            node->addChild(parser.parseStatement());
        }

        return node;
    }

    std::string AtomicStatementNode::name() const {
        return "AtomicStatement";
    }

    bool AtomicStatementNode::strict() const {
        return _strict;
    }

    void AtomicStatementNode::codeGenCSource(CSourceContext& context) {
        context << "if (three_begin_transaction(";
        context << (this->strict() ? "true" : "false");
        context.current()->printLineAndIndent(")) {");

        this->codeGenCSourceForChildren(context);

        context.current()->printLine("three_end_transaction();");

        if (this->_elseNode) {
            context.current()->decreaseIndentation();
            context.current()->printLineAndIndent("} else {");
            this->_elseNode->codeGenCSourceForChildren(context);
        }

        context.current()->outdentAndPrintLine("}");
    }
}
