#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/DataType.h"

namespace Three {
    class MetafunctionNode : public ASTNode {
    public:
        static MetafunctionNode* parse(Parser& parser);

    public:
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
