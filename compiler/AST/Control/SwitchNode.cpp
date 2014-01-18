#include "SwitchNode.h"
#include "CaseNode.h"
#include "../../Parser.h"
#include "ElseNode.h"

#include <assert.h>

namespace Three {
    SwitchNode* SwitchNode::parse(Parser& parser) {
        SwitchNode* node = new SwitchNode();

        assert(parser.next().type() == Token::Type::KeywordSwitch);

        node->_argumentNode = parser.parseExpression();
        node->_elseNode = nullptr;
        parser.parseNewline();

        parser.parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::KeywordEnd:
                    return true;
                case Token::Type::KeywordElse:
                    node->_elseNode = ElseNode::parse(parser);
                    return true;
                default:
                    node->addChild(CaseNode::parse(parser));
                    return false;
                }
        });

        parser.nextIf("end");

        return node;
    }

    std::string SwitchNode::name() const {
        return "Switch";
    }

    ASTNode* SwitchNode::argument() const {
        return this->_argumentNode;
    }

    void SwitchNode::codeGen(CSourceContext& context) {
        this->eachChild([&] (ASTNode* child, uint32_t index) {
            CaseNode* caseNode = dynamic_cast<CaseNode*>(child);

            if (index != 0) {
                context << "} else ";
            }

            context << "if (";
            this->argument()->codeGen(context);
            context << " == ";
            caseNode->argument()->codeGen(context);
            context.current()->printLineAndIndent(") {");

            caseNode->codeGen(context);

            context.current()->decreaseIndentation();
        });

        if (_elseNode) {
            context.current()->printLineAndIndent("} else {");

            _elseNode->codeGenChildren(context);

            context.current()->decreaseIndentation();
        }

        context.current()->printLine("}");
    }
}
