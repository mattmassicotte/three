#pragma once

#include "../ASTNode.h"

namespace Three {
    class NamespaceNode : public ASTNode {
    public:
        static NamespaceNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;

    private:
        std::string _name;
    };
}
