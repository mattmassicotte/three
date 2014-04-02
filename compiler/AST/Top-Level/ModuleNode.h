#pragma once

#include "../ASTNode.h"

namespace Three {
    class ModuleNode : public ASTNode {
    public:
        virtual std::string name() const;
        std::string moduleName() const;

    private:
        std::string _name;
    };
}
