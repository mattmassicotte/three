#include "MemberAccessNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>

namespace Three {
    MemberAccessNode* MemberAccessNode::parse(NewParser& parser, ASTNode* operand) {
        bool indirect = false;

        switch (parser.helper()->peek().type()) {
            case Token::Type::OperatorDot:
                break;
            case Token::Type::OperatorArrow:
                indirect = true;
                break;
            default:
                assert(0 && "Message: expected a . or -> token");
                return nullptr;
        }

        MemberAccessNode* node = new MemberAccessNode();

        parser.helper()->next();

        node->_indirect = indirect;

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: a member access operator must be followed by an identifier");
            return nullptr;
        }

        node->_memberName = parser.helper()->nextStr();

        node->addChild(operand);

        return node;
    }

    std::string MemberAccessNode::nodeName() const {
        return "Member Access Operator";
    }

    void MemberAccessNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string MemberAccessNode::name() const {
        return _memberName;
    }

    std::string MemberAccessNode::memberName() const {
        return _memberName;
    }

    bool MemberAccessNode::indirect() const {
        return _indirect;
    }
}
