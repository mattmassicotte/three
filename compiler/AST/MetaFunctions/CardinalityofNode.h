#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class CardinalityofNode : public MetafunctionNode {
    public:
        static CardinalityofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        DataType dataTypeArgument;
    };
}
