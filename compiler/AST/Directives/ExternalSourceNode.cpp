#include "ExternalSourceNode.h"

namespace Three {
    void ExternalSourceNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string ExternalSourceNode::argument() const {
        return _argument;
    }

    TranslationUnit::Visibility ExternalSourceNode::visibility() const {
        return _visibility;
    }
}
