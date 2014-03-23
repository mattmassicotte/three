#pragma once

#include "OperatorNode.h"

namespace Three {
    class IndexerNode : public OperatorNode {
    public:
        static IndexerNode* parse(Parser& parser, ASTNode* operand);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
