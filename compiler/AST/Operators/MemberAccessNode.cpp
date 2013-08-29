#include "MemberAccessNode.h"

namespace Three {
    MemberAccessNode::MemberAccessNode(const std::string& name, bool indirect) :
        _memberName(name), _indirect(indirect) {
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

    void MemberAccessNode::codeGenCSource(CSourceContext& context) {
        this->childAtIndex(0)->codeGenCSource(context);

        if (this->indirect()) {
            context << "->";
        } else {
            context << ".";
        }

        context << this->memberName();
    }
}
