#pragma once

#include "../ASTNode.h"

namespace Three {
    class LoopNode : public ASTNode {
    public:
        static LoopNode* parse(Parser& parser);

    public:
        virtual ~LoopNode();
        virtual std::string name() const;

        ASTNode* condition() const;
        bool evaluateConditionAtEnd() const;

        void codeGenCSource(CSourceContext& context);

    private:
        void codeGenCWhile(CSourceContext& context);

    private:
        ASTNode* _condition;
        bool _evaluateConditionAtEnd;
    };
}
