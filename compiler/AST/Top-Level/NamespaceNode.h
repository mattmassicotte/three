#pragma once

#include "../ASTNode.h"

namespace Three {
    class NamespaceNode : public ASTNode {
    public:
        static NamespaceNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;

    private:
        std::string _name;
    };
}
