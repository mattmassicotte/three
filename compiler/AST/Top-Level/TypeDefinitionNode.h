#pragma once

#include "DefinitionNode.h"

namespace Three {
    class TypeDefinitionNode : public DefinitionNode {
    public:
        virtual std::string name() const;

        std::string destTypeName() const;

    private:
        std::string   _destTypeName;
    };
}
