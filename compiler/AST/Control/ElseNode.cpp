#include "ElseNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    ElseNode* ElseNode::parse(NewParser& parser) {
        assert(parser.helper()->nextIf(Token::Type::KeywordElse));

        ElseNode* node = new ElseNode();

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: else must be followed by a newline");
            return node;
        }

        // parse the body
        bool success = parser.helper()->parseUntil(false, [&] (const Token& token) {
            switch (token.type()) {
                case Token::Type::KeywordEnd:
                    return true;
                default:
                    node->addChild(parser.parseStatement());
                    return false;
                }
        });

        if (!success) {
            assert(0 && "Message: unable to parse else body");
        }

        return node;
    }

    std::string ElseNode::nodeName() const {
        return "Else";
    }

    std::string ElseNode::name() const {
        return "Else";
    }

    void ElseNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }
}
