#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class TypeofNode : public MetafunctionNode {
    public:
        static TypeofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        DataType dataTypeArgument;
    };
}
