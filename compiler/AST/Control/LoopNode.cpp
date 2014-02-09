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

    void LoopNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    ASTNode* LoopNode::condition() const {
        return _condition;
    }

    bool LoopNode::evaluateConditionAtEnd() const {
        return _evaluateConditionAtEnd;
    }

    void LoopNode::codeGen(CSourceContext& context) {
        // infinite loop
        if (!this->condition()) {
            context.current()->printLineAndIndent("for (;;) {");

            this->codeGenChildren(context);

            context.current()->outdentAndPrintLine("}");

            return;
        }

        // do-while
        if (this->evaluateConditionAtEnd()) {
            context.current()->printLineAndIndent("do {");

            this->codeGenChildren(context);

            context.current()->print("} while (");
            this->condition()->codeGen(context);
            context.current()->outdentAndPrintLine(");");

            return;
        }

        // regular while
        context.current()->print("while (");
        this->condition()->codeGen(context);
        context.current()->printLineAndIndent(") {");

        this->codeGenChildren(context);

        context.current()->outdentAndPrintLine("}");
    }
}
