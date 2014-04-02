#pragma once

#include "ASTNode.h"

namespace Three {
    class RootNode : public ASTNode {
    public:
        virtual std::string name() const;
    };
}
