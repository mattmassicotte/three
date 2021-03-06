#pragma once

#include "OperatorNode.h"

namespace Three {
    class IndexerNode : public OperatorNode {
    public:
        static IndexerNode* parse(Parser& parser, ASTNode* operand);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
