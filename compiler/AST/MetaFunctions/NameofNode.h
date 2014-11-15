#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class NameofNode : public MetafunctionNode {
    public:
        static NameofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        DataType dataTypeArgument;
    };
}
