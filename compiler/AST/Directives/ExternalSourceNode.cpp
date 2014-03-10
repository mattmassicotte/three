#include "ExternalSourceNode.h"

namespace Three {
    void ExternalSourceNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    TranslationUnit::Visibility ExternalSourceNode::visibility() const {
        return _visibility;
    }
}
