#include "IfNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    IfNode* IfNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordIf));

        IfNode* node = new IfNode();

        // parse the condition
        node->_conditionNode = parser.parseExpression();
        if (!node->_conditionNode) {
            assert(0 && "Message: failed to parse if condition");
            return node;
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: new line after if expected");
            return node;
        }

        if (!parser.parseBodyWithElse(node, &node->_elseNode)) {
            assert(0 && "Message: unable to parse if body");
            return node;
        }

        return node;
    }

    ASTNode* IfNode::parseTailing(Parser& parser, ASTNode* node) {
        if (!parser.helper()->nextIf(Token::Type::KeywordIf)) {
            return node;
        }

        IfNode* ifNode = new IfNode();

        ifNode->addChild(node);

        ifNode->setCondition(parser.parseExpression());

        return ifNode;
    }

    IfNode::IfNode() : _elseNode(nullptr) {
    }

    std::string IfNode::nodeName() const {
        return "If";
    }

    std::string IfNode::name() const {
        return "If";
    }

    void IfNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    ASTNode* IfNode::condition() const {
        return _conditionNode;
    }

    void IfNode::setCondition(ASTNode* node) {
        _conditionNode = node;
    }

    ASTNode* IfNode::elseStatement() const {
        return _elseNode;
    }
}
