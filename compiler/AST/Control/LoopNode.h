#pragma once

#include "../ASTNode.h"

namespace Language {
    class LoopNode : public ASTNode {
    public:
        static LoopNode* parse(Parser& parser);

    public:
        virtual ~LoopNode();
        virtual std::string name() const;

        ASTNode* condition() const;

        void codeGenCSource(CSourceContext& context);

    public:
        ASTNode* _condition;
    };
}
