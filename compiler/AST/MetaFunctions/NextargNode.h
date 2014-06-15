#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class NextargNode : public MetafunctionNode {
    public:
        static NextargNode* parse(Parser& parser);

    public:
        std::string name() const;
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
