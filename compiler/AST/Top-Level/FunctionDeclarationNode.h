#pragma once

#include "DefinitionNode.h"

namespace Three {
    class FunctionDeclarationNode : public DefinitionNode {
    public:
        virtual std::string name() const;
    };
}
