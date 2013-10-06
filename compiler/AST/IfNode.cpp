#include "IfNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Three {
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

    ASTNode* IfNode::parseTailing(Parser& parser, ASTNode* node) {
        if (parser.peek().type() != Token::Type::KeywordIf) {
            return node;
        }

        assert(parser.next().type() == Token::Type::KeywordIf);

        IfNode* ifNode = new IfNode();

        ifNode->addChild(node);

        ifNode->setCondition(parser.parseExpression());

        return ifNode;
    }

    IfNode::IfNode() : _elseNode(NULL) {
    }

    std::string IfNode::name() const {
        return "If";
    }

    ASTNode* IfNode::condition() const {
        return _conditionNode;
    }

    void IfNode::setCondition(ASTNode* node) {
        _conditionNode = node;
    }

    ElseNode* IfNode::elseStatement() const {
        return _elseNode;
    }

    void IfNode::codeGenCSource(CSourceContext& context) {
        context.current()->print("if ");

        // TODO: this is a messy hack to avoid duplicating parenthesis around
        // operators
        if (this->condition()->nodeName() != "Operator") {
            context.current()->print("(");
        }

        this->condition()->codeGenCSource(context);

        if (this->condition()->nodeName() != "Operator") {
            context.current()->print(")");
        }

        context.current()->printLineAndIndent(" {");

        this->codeGenCSourceForChildren(context);

        if (this->elseStatement()) {
            context.current()->outdentAndPrintLine("} else {");
            context.current()->increaseIndentation();

            this->elseStatement()->codeGenCSourceForChildren(context);
        }

        context.current()->outdentAndPrintLine("}");
    }
}
