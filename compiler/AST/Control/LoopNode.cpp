#include "LoopNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    LoopNode* LoopNode::parse(Parser& parser) {
        LoopNode* node = new LoopNode();

        assert(parser.next().type() == Token::Type::KeywordLoop);

        if (parser.peek().type() == Token::Type::Newline) {
            node->_condition = NULL;
        } else {
            node->_condition = parser.parseExpression();
        }

        parser.parseNewline();

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    LoopNode::~LoopNode() {
        delete _condition;
    }

    std::string LoopNode::name() const {
        return "Loop";
    }

    ASTNode* LoopNode::condition() const {
        return _condition;
    }

    void LoopNode::codeGenCSource(CSourceContext& context) {
        context << "while (";
        if (!this->condition()) {
            context << "1";
        } else {
            this->condition()->codeGenCSource(context);
        }

        context.current()->printLineAndIndent(") {");

        this->codeGenCSourceForChildren(context);

        context.current()->outdentAndPrintLine("}");
    }
}
