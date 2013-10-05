#pragma once

#include "../ASTNode.h"
#include "../ElseNode.h"

namespace Language {
    class AtomicStatementNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
