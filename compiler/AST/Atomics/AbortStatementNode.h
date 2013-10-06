#pragma once

#include "../ASTNode.h"

namespace Three {
    class AbortStatementNode : public ASTNode {
    public:
        static AbortStatementNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

    public:
        void codeGenCSource(CSourceContext& context);
    };
}
