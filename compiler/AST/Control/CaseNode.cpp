#include "CaseNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    CaseNode* CaseNode::parse(Parser& parser) {
        CaseNode* node = new CaseNode();

        assert(parser.next().type() == Token::Type::KeywordCase);

        node->_argumentNode = parser.parseExpression();
        parser.parseNewline();

        parser.parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::KeywordEnd:
                case Token::Type::KeywordCase:
                case Token::Type::KeywordElse:
                    return true;
                default:
                    node->addChild(parser.parseStatement());
                    return false;
                }
        });

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
