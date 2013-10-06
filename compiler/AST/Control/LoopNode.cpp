#include "LoopNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    LoopNode* LoopNode::parse(Parser& parser) {
        LoopNode* node = new LoopNode();

        assert(parser.next().type() == Token::Type::KeywordLoop);

        node->_evaluateConditionAtEnd = false;

        switch (parser.peek().type()) {
            case Token::Type::Newline:
                node->_condition = NULL;
                break;
            case Token::Type::PunctuationColon:
                parser.next();
                assert(parser.next().str() == "after");
                node->_evaluateConditionAtEnd = true;

                node->_condition = parser.parseExpression();
                break;
            default:
                node->_condition = parser.parseExpression();
                break;
        }

        parser.parseNewline();

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    LoopNode::~LoopNode() {
        if (_condition) {
            delete _condition;
        }
    }

    std::string LoopNode::name() const {
        return "Loop";
    }

    ASTNode* LoopNode::condition() const {
        return _condition;
    }

    bool LoopNode::evaluateConditionAtEnd() const {
        return _evaluateConditionAtEnd;
    }

    void LoopNode::codeGenCWhile(CSourceContext& context) {
        context << "while (";
        this->condition()->codeGenCSource(context);
        context << ")";
    }

    void LoopNode::codeGenCSource(CSourceContext& context) {
        if (!this->condition()) {
            context.current()->print("for (;;)");

            context.current()->printLineAndIndent(" {");
            this->codeGenCSourceForChildren(context);
            context.current()->outdentAndPrintLine("}");

            return;
        }

        if (this->evaluateConditionAtEnd()) {
            context.current()->print("do");
            context.current()->printLineAndIndent(" {");

            this->codeGenCSourceForChildren(context);

            context.current()->print("} ");

            this->codeGenCWhile(context);
            context.current()->outdentAndPrintLine(";");

            return;
        }

        this->codeGenCWhile(context);
        context.current()->printLineAndIndent(" {");
        this->codeGenCSourceForChildren(context);
        context.current()->outdentAndPrintLine("}");
    }
}
