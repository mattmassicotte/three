#include "AtomicExpressionNode.h"
#include "../Operators/OperatorNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    AtomicExpressionNode* AtomicExpressionNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordAtomic));

        AtomicExpressionNode* node = new AtomicExpressionNode();

        AtomicNode::parseOrdering(parser, node);

        if (!parser.helper()->nextIf(Token::Type::PunctuationOpenParen)) {
            assert(0 && "Message: expecting open paren for atomic expression");
        }

        node->addChild(parser.parseExpression());

        if (!parser.helper()->nextIf(Token::Type::PunctuationCloseParen)) {
            assert(0 && "Message: expecting close paren for atomic expression");
        }

        return node;
    }

    std::string AtomicExpressionNode::c11AtomicFunctionForFullOperation(const std::string& op) {
        if (op == "+=") {
            return "atomic_fetch_add_explicit";
        } else if (op == "-=") {
            return "atomic_fetch_sub_explicit";
        }

        return "";
    }

    bool AtomicExpressionNode::c11AtomicFunctionIsLoadOperation(const std::string& op) {
        // >, >=, <, <=, ==
        return op == ">" || op == ">=" || op == "<" || op == "<=" || op == "==";
    }

    std::string AtomicExpressionNode::nodeName() const {
        return "Atomic Expression";
    }

    std::string AtomicExpressionNode::name() const {
        return "AtomicExpression";
    }

    void AtomicExpressionNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    OperatorNode* AtomicExpressionNode::op() const {
        return dynamic_cast<OperatorNode*>(this->childAtIndex(0));
    }
}
