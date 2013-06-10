#include "AtomicExpressionNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Language {
    AtomicExpressionNode* AtomicExpressionNode::parse(Parser& parser, std::string type) {
        AtomicExpressionNode* node = new AtomicExpressionNode();

        if (type.size() == 0) {
            assert(parser.next().type() == Token::Type::KeywordAtomic);

            if (parser.nextIf(":")) {
                node->_type = parser.next().str();
            } else {
                node->_type = "ordered";
            }
        } else {
            node->_type = type;
        }

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        node->addChild(parser.parseExpression());

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

        return node;
    }

    std::string AtomicExpressionNode::name() const {
        return "AtomicExpression";
    }

    std::string AtomicExpressionNode::type() const {
        return _type;
    }

    void AtomicExpressionNode::codeGenCSource(CSourceContext& context) {
    }
}
