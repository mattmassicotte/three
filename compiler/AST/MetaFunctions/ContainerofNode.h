#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class ContainerofNode : public MetafunctionNode {
    public:
        static ContainerofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        DataType dataTypeArgument;
        std::string memberName;
    };
}
