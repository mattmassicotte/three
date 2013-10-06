#include "SwitchNode.h"
#include "CaseNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    SwitchNode* SwitchNode::parse(Parser& parser) {
        SwitchNode* node = new SwitchNode();

        assert(parser.next().type() == Token::Type::KeywordSwitch);

        node->_argumentNode = parser.parseExpression();
        parser.parseNewline();

        parser.parseUntilEnd([&] () {
            node->addChild(CaseNode::parse(parser));
        });

        return node;
    }

    std::string SwitchNode::name() const {
        return "Switch";
    }

    ASTNode* SwitchNode::argument() const {
        return this->_argumentNode;
    }

    void SwitchNode::codeGenCSource(CSourceContext& context) {
        this->eachChild([&] (ASTNode* child, uint32_t index) {
            CaseNode* caseNode = dynamic_cast<CaseNode*>(child);

            if (index != 0) {
                context << "} else ";
            }

            context << "if (";
            this->argument()->codeGenCSource(context);
            context << " == ";
            caseNode->argument()->codeGenCSource(context);
            context.current()->printLineAndIndent(") {");

            caseNode->codeGenCSource(context);

            if (index == this->childCount() - 1) {
                context.current()->outdentAndPrintLine("}");
            } else {
                context.current()->decreaseIndentation();
            }
        });
    }
}
