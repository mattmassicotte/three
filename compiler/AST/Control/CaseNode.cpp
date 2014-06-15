#include "CaseNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    CaseNode* CaseNode::parse(Parser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordCase));

        CaseNode* node = new CaseNode();

        node->_argumentNode = parser.parseExpression();

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: case argument must be followed by a newline");
            return node;
        }

        bool success = parser.helper()->parseUntil(false, [&] (const Token& token) {
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

        if (!success) {
            assert(0 && "Message: unable to parse case body");
        }

        return node;
    }

    std::string CaseNode::nodeName() const {
        return "Case";
    }

    std::string CaseNode::name() const {
        return "Case";
    }

    ASTNode* CaseNode::argument() const {
        return this->_argumentNode;
    }
}
