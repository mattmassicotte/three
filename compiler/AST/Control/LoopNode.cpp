#include "LoopNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    LoopNode* LoopNode::parse(Parser& parser) {
        LoopNode* node = new LoopNode();

        assert(parser.next().type() == Token::Type::KeywordLoop);
        parser.parseNewline();

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    std::string LoopNode::name() const {
        return "Loop";
    }

    void LoopNode::codeGenCSource(CSourceContext& context) {
        context.current()->printLineAndIndent("while (1) {");

        this->codeGenCSourceForChildren(context);

        context.current()->outdentAndPrintLine("}");
    }
}
