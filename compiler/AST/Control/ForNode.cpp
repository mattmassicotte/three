#include "ForNode.h"
#include "../../Parser.h"
#include "../VariableDeclarationNode.h"

#include <assert.h>

namespace Three {
    ForNode* ForNode::parse(Parser& parser) {
        ForNode* node = new ForNode();

        assert(parser.next().type() == Token::Type::KeywordFor);
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->_evaluateConditionAtEnd = false;

        if (parser.isAtType()) {
            node->_startExpression = VariableDeclarationNode::parse(parser);
        } else {
            node->_startExpression = parser.parseExpression();
        }

        if (parser.nextIf("in")) {
            node->_inExpression = parser.parseExpression();
            node->_condition = NULL;
            node->_loopExpression = NULL;
        } else {
            node->_inExpression = NULL;
            assert(parser.next().type() == Token::Type::PunctuationSemicolon);
            node->_condition = parser.parseExpression();
            assert(parser.next().type() == Token::Type::PunctuationSemicolon);
            node->_loopExpression = parser.parseExpression();
        }

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);
        parser.parseNewline();

        parser.parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    ForNode::~ForNode() {
        if (_startExpression) {
            delete _startExpression;
        }

        if (_condition) {
            delete _condition;
        }

        if (_loopExpression) {
            delete _loopExpression;
        }
    }

    std::string ForNode::name() const {
        return "For";
    }

    ASTNode* ForNode::startExpression() const {
        return _startExpression;
    }

    ASTNode* ForNode::condition() const {
        return _condition;
    }

    ASTNode* ForNode::loopExpression() const {
        return _loopExpression;
    }

    bool ForNode::evaluateConditionAtEnd() const {
        return _evaluateConditionAtEnd;
    }

    void ForNode::codeGenCSource(CSourceContext& context) {
        context << "for (";
        this->startExpression()->codeGenCSource(context);

        // TODO: its incorrect to not include a semi-colon here. But!  VariableDeclarations already do that.

        this->condition()->codeGenCSource(context);
        context << "; ";
        this->loopExpression()->codeGenCSource(context);

        context.current()->printLineAndIndent(") {");
        this->codeGenCSourceForChildren(context);
        context.current()->outdentAndPrintLine("}");
    }
}
