#pragma once

#include "../ASTNode.h"
#include "../ElseNode.h"

namespace Three {
    class AtomicStatementNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
