#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class OffsetofNode : public MetafunctionNode {
    public:
        static OffsetofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        DataType dataTypeArgument;
        std::string memberName;
    };
}
