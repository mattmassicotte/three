#pragma once

#include "CompositeTypeDefinitionNode.h"

namespace Three {
    class UnionNode : public CompositeTypeDefinitionNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

    protected:
        void setParsedIntegerSpecifier(Parser& parser, uint32_t value);
    };
}
