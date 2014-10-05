#pragma once

#include "../ASTNode.h"
#include "compiler/Constructs/QualifiedName.h"

namespace Three {
    class NamespaceNode : public ASTNode {
    public:
        static NamespaceNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        QualifiedName qualifiedName() const;

    private:
        QualifiedName _name;
    };
}
