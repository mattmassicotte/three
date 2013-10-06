#pragma once

#include "OperatorNode.h"

namespace Three {
    class IndexerNode : public OperatorNode {
    public:
        static IndexerNode* parse(Parser& parser, ASTNode* operand);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
