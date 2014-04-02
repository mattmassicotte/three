#pragma once

#include "../ASTNode.h"

namespace Three {
    class LinkageNode : public ASTNode {
    public:
        virtual std::string name() const;

        std::string libraryName() const;

    private:
        std::string _libraryName;
    };
}
