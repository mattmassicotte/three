#pragma once

#include "../ASTNode.h"

namespace Three {
    class BreakNode : public ASTNode {
    public:
        static BreakNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
