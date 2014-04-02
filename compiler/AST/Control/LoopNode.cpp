#include "LoopNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    LoopNode* LoopNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordLoop));

        LoopNode* node = new LoopNode();

        node->_evaluateConditionAtEnd = false;
        node->_condition = nullptr;

        switch (parser.helper()->peek().type()) {
            case Token::Type::Newline:
                node->_condition = nullptr;
                break;
            case Token::Type::PunctuationColon:
                parser.helper()->next();
                if (!parser.helper()->nextIf("after")) {
                    assert(0 && "Message: loop can only have an after specifier");
                }

                node->_evaluateConditionAtEnd = true;

                node->_condition = parser.parseExpression();
                break;
            default:
                node->_condition = parser.parseExpression();
                break;
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: loop must be followed by a newline");
            return node;
        }

        parser.helper()->parseUntilEnd([&] () {
            node->addChild(parser.parseStatement());
        });

        return node;
    }

    LoopNode::~LoopNode() {
        if (_condition) {
            delete _condition;
        }
    }

    std::string LoopNode::nodeName() const {
        return "Loop";
    }

    std::string LoopNode::name() const {
        return "Loop";
    }

    void LoopNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    ASTNode* LoopNode::condition() const {
        return _condition;
    }

    bool LoopNode::evaluateConditionAtEnd() const {
        return _evaluateConditionAtEnd;
    }
}
