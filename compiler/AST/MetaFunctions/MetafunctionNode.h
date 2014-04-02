#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewDataType.h"

namespace Three {
    class MetafunctionNode : public ASTNode {
    public:
        static MetafunctionNode* parse(NewParser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        NewDataType dataTypeArgument;
    };
}
