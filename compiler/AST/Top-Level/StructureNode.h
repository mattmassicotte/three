#pragma once

#include "CompositeTypeDefinitionNode.h"

namespace Three {
    class StructureNode : public CompositeTypeDefinitionNode {
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        uint32_t packing() const;

    protected:
        void setParsedIntegerSpecifier(Parser& parser, uint32_t value);

    private:
        uint32_t _packing;
    };
}
