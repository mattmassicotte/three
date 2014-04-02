#pragma once

#include "DefinitionNode.h"

namespace Three {
    class ValueDefinitionNode : public DefinitionNode {
    public:
        virtual std::string name() const;

        std::string definedName() const;

    private:
        std::string _definedName;
    };
}
