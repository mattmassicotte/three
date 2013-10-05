#include "AtomicStatementNode.h"
#include "../../Parser.h"
#include "AtomicExpressionNode.h"

#include <assert.h>

namespace Language {
    ASTNode* AtomicStatementNode::parse(Parser& parser) {
        std::string type;

        AtomicStatementNode* node = new AtomicStatementNode();

        assert(parser.peek().type() == Token::Type::KeywordAtomic);

        // we need to check for an atomic statement
        // case #1 "atomic (...)"
        if (parser.peek(2).type() == Token::Type::PunctuationOpenParen) {
            return AtomicExpressionNode::parse(parser);
        }

        // case #1 "atomic:ordering (...)" where "ordering" != fallback
        if (parser.peek(2).type() == Token::Type::PunctuationColon) {
            if (parser.peek(3).str() != "fallback") {
                return AtomicExpressionNode::parse(parser);
            }
        }

        assert(parser.next().type() == Token::Type::KeywordAtomic);

        if (parser.nextIf(":")) {
            type = parser.next().str();
        } else {
            type = "ordered";
        }

        node->_strict = type == "strict";

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
        context.addHeader("stdbool.h");
        context.addHeader("three/runtime/transactional_memory.h");

        context << "if (three_transaction_begin(";
        context << (this->strict() ? "true" : "false");
        context.current()->printLineAndIndent(")) {");

        this->codeGenCSourceForChildren(context);

        context << "three_transaction_end(";
        context << (this->strict() ? "true" : "false");
        context.current()->printLine(");");

        if (this->_elseNode) {
            context.current()->decreaseIndentation();
            context.current()->printLineAndIndent("} else {");
            this->_elseNode->codeGenCSourceForChildren(context);
        }

        context.current()->outdentAndPrintLine("}");
    }
}
