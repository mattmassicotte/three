#pragma once

#include "ASTNode.h"

namespace Language {
    class OperatorNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser, uint32_t precedence, ASTNode* leftOperand);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
