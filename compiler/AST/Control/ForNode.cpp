#include "ForNode.h"
#include "../Variables/VariableDeclarationNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    ForNode* ForNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordFor));

        ForNode* node = new ForNode();

        if (!parser.helper()->nextIf(Token::Type::PunctuationOpenParen)) {
            assert(0 && "Message: for loop must start with an open-paren");
        }

        bool foundVariable = parser.isAtType();
        if (foundVariable) {
            node->_startExpression = VariableDeclarationNode::parse(parser);
            node->_startExpression->setStatement(false);
        } else {
            node->_startExpression = parser.parseExpression();
        }

        if (parser.helper()->nextIf("in")) {
            node->_condition = nullptr;
            node->_loopExpression = nullptr;

            if (!foundVariable) {
                assert(0 && "Message: A ranged for loop must define a local variable");
            }

            node->_rangeStartExpression = parser.parseExpression();

            if (!parser.helper()->nextIf(Token::Type::PunctuationColon)) {
                assert(0 && "Message: A range must be seperated by a colon");
            }

            node->_rangeEndExpression = parser.parseExpression();
        } else {
            node->_rangeStartExpression = nullptr;
            node->_rangeEndExpression = nullptr;

            if (!parser.helper()->nextIf(Token::Type::PunctuationSemicolon)) {
                assert(0 && "Message: for loop expected a semi-colon");
            }

            node->_condition = parser.parseExpression();

            if (!parser.helper()->nextIf(Token::Type::PunctuationSemicolon)) {
                assert(0 && "Message: for loop expected a semi-colon");
            }

            node->_loopExpression = parser.parseExpression();
        }

        if (!parser.helper()->nextIf(Token::Type::PunctuationCloseParen)) {
            assert(0 && "Message: for loop should finish with a close-paren");
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: for loop should end with a newline");
        }

        parser.helper()->parseUntilEnd([&] () {
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

    std::string ForNode::nodeName() const {
        return "For";
    }

    std::string ForNode::name() const {
        return "For";
    }

    void ForNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
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

    NewVariable* ForNode::rangeLoopVariable() const {
        return dynamic_cast<VariableDeclarationNode*>(this->startExpression())->variable();
    }
}
