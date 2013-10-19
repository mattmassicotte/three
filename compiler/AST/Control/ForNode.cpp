#include "ForNode.h"
#include "../../Parser.h"
#include "../Variables/VariableDeclarationNode.h"

#include <assert.h>

namespace Three {
    ForNode* ForNode::parse(Parser& parser) {
        ForNode* node = new ForNode();

        assert(parser.next().type() == Token::Type::KeywordFor);
        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->_evaluateConditionAtEnd = false;

        bool foundVariable = parser.isAtType();
        if (foundVariable) {
            node->_startExpression = VariableDeclarationNode::parse(parser);
            node->_startExpression->setStatement(false);
        } else {
            node->_startExpression = parser.parseExpression();
        }

        if (parser.nextIf("in")) {
            node->_condition = NULL;
            node->_loopExpression = NULL;

            assert(foundVariable && "A ranged for loop must define a local variable");

            node->_rangeStartExpression = parser.parseExpression();
            assert(parser.next().type() == Token::Type::PunctuationColon);
            node->_rangeEndExpression = parser.parseExpression();
        } else {
            node->_rangeStartExpression = NULL;
            node->_rangeEndExpression = NULL;
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

    ASTNode* ForNode::rangeStartExpression() const {
        return _rangeStartExpression;
    }

    ASTNode* ForNode::rangeEndExpression() const {
        return _rangeEndExpression;
    }

    Variable* ForNode::rangeLoopVariable() const {
        return dynamic_cast<VariableDeclarationNode*>(this->startExpression())->variable();
    }

    bool ForNode::evaluateConditionAtEnd() const {
        return _evaluateConditionAtEnd;
    }

    void ForNode::codeGen(CSourceContext& context) {
        context << "for (";

        this->codeGenStartExpression(context);
        context << "; ";

        this->codeGenCondition(context);
        context << "; ";

        this->codeGenLoopExpression(context);

        context.current()->printLineAndIndent(") {");
        this->codeGenChildren(context);
        context.current()->outdentAndPrintLine("}");
    }

    void ForNode::codeGenCondition(CSourceContext& context) const {
        if (this->condition()) {
            this->condition()->codeGen(context);
            return;
        }

        assert(_rangeStartExpression && _rangeEndExpression);

        context << "(";
        context << this->rangeLoopVariable()->name();
        context << " < ";

        this->rangeEndExpression()->codeGen(context);

        context << ") && (";

        this->rangeStartExpression()->codeGen(context);

        context << " < ";

        this->rangeEndExpression()->codeGen(context);

        context << ")";
    }

    void ForNode::codeGenStartExpression(CSourceContext& context) const {
        this->startExpression()->codeGen(context);

        if (_rangeStartExpression) {
            context << " = ";
            this->rangeStartExpression()->codeGen(context);
        }
    }

    void ForNode::codeGenLoopExpression(CSourceContext& context) const {
        if (this->loopExpression()) {
            this->loopExpression()->codeGen(context);
            return;
        }

        assert(_rangeStartExpression && _rangeEndExpression);

        context << "++" << this->rangeLoopVariable()->name();
    }
}
