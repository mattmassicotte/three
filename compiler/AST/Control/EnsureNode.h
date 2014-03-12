#pragma once

#include "../ASTNode.h"

namespace Three {
    class EnsureNode : public ASTNode {
    public:
        static EnsureNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
