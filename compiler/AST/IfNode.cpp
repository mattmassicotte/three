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

        assert(parser.next().type() == Token::Type::Newline);

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
        context.print("if (");

        this->condition()->codeGenCSource(context);

        context.print(") {");
        context.printNewLine(CSourceContext::Indentation::Indent);

        this->codeGenCSourceForChildren(context);

        context.printNewLine(CSourceContext::Indentation::Outdent);

        if (this->elseStatement()) {
            context.print("} else {");
            context.printNewLine(CSourceContext::Indentation::Indent);

            this->elseStatement()->codeGenCSourceForChildren(context);

            context.printNewLine(CSourceContext::Indentation::Outdent);
        }

        context.print("}");
        context.printNewLine();
    }
}
