#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class ABIofNode : public MetafunctionNode {
    public:
        static ABIofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
