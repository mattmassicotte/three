#include "CaseNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    CaseNode* CaseNode::parse(Parser& parser) {
        CaseNode* node = new CaseNode();

        assert(parser.next().type() == Token::Type::KeywordCase);

        node->_argumentNode = parser.parseExpression();
        parser.parseNewline();

        while (true) {
            if (parser.peek().type() == Token::Type::KeywordCase) {
                break;
            }

            if (parser.peek().type() == Token::Type::KeywordEnd) {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        return node;
    }

    std::string CaseNode::name() const {
        return "Case";
    }

    ASTNode* CaseNode::argument() const {
        return this->_argumentNode;
    }

    void CaseNode::codeGen(CSourceContext& context) {
        this->codeGenChildren(context);
    }
}
