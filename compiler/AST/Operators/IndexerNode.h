#pragma once

#include "OperatorNode.h"

namespace Three {
    class IndexerNode : public OperatorNode {
    public:
        static IndexerNode* parse(NewParser& parser, ASTNode* operand);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
