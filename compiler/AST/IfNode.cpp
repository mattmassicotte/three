#include "IfNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    IfNode* IfNode::parse(Parser& parser) {
        IfNode* node = new IfNode();

        node->setStatement(true);

        assert(parser.next().type() == Token::Type::KeywordIf);

        // parse the condition
        node->_conditionNode = parser.parseExpression();

        parser.parseNewline();

        // and now, parse the body
        while (true) {
            Token t = parser.peek();

            if (t.type() == Token::Type::KeywordEnd || t.type() == Token::Type::KeywordElse) {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        if (parser.peek().type() == Token::Type::KeywordElse) {
            node->_elseNode = ElseNode::parse(parser);
        } else {
            assert(parser.next().type() == Token::Type::KeywordEnd);
        }

        return node;
    }

    IfNode::IfNode() : _elseNode(NULL) {
    }

    std::string IfNode::name() const {
        return "If";
    }

    ASTNode* IfNode::condition() const {
        return _conditionNode;
    }

    ElseNode* IfNode::elseStatement() const {
        return _elseNode;
    }

    void IfNode::codeGenCSource(CSourceContext& context) {
        context.current()->print("if (");

        this->condition()->codeGenCSource(context);

        context.current()->printLineAndIndent(") {");

        this->codeGenCSourceForChildren(context);

        context.current()->decreaseIndentation();
        context.current()->printNewLine();

        if (this->elseStatement()) {
            context.current()->print("} else {");
            context.current()->increaseIndentation();
            context.current()->printNewLine();

            this->elseStatement()->codeGenCSourceForChildren(context);

            context.current()->decreaseIndentation();
            context.current()->printNewLine();
        }

        context.current()->print("}");
    }
}
