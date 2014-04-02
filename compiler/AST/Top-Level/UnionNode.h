#pragma once

#include "CompositeTypeDefinitionNode.h"

namespace Three {
    class UnionNode : public CompositeTypeDefinitionNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

    protected:
        void setParsedIntegerSpecifier(NewParser& parser, uint32_t value);
    };
}
