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

    void IfNode::renderCCode(std::stringstream& stream, uint32_t indentation) {
        stream << std::string(indentation*4, ' ');

        stream << "if (";

        this->condition()->renderCCode(stream, 0);

        stream << ") {" << std::endl;

        this->renderChildrenCCode(stream, indentation+1);

        stream << std::string(indentation*4, ' ');

        if (this->elseStatement()) {
            stream << "} else {" << std::endl;

            this->elseStatement()->renderChildrenCCode(stream, indentation+1);

            stream << std::string(indentation*4, ' ');
        }

        stream << "}" << std::endl;
    }
}
