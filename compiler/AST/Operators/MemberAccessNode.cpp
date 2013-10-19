#include "MemberAccessNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    MemberAccessNode* MemberAccessNode::parse(Parser& parser, ASTNode* operand) {
        MemberAccessNode* node = new Three::MemberAccessNode();

        std::string op = parser.next().str();

        // must be a valid op, plus identifier
        assert(op == "->" || op == ".");
        assert(parser.peek().type() == Token::Type::Identifier);

        node->_memberName = parser.next().str();
        node->_indirect = op == "->";

        node->addChild(operand);

        return node;
    }

    TypeReference MemberAccessNode::nodeType() const {
        // This is tricky.  We need to look up the type we reference, and then
        // get the type of the member itself.
        TypeReference type = this->childAtIndex(0)->nodeType();

        return type.referencedType()->childForName(this->memberName());
    }

    std::string MemberAccessNode::name() const {
        return "MemberAccess";
    }

    std::string MemberAccessNode::memberName() const {
        return _memberName;
    }

    bool MemberAccessNode::indirect() const {
        return _indirect;
    }

    void MemberAccessNode::codeGen(CSourceContext& context) {
        this->childAtIndex(0)->codeGen(context);

        if (this->indirect()) {
            context << "->";
        } else {
            context << ".";
        }

        context << this->memberName();
    }
}
