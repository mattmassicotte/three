#pragma once

#include "../ASTNode.h"

using namespace Language;

namespace Three {
    class ForNode : public ASTNode {
    public:
        static ForNode* parse(Parser& parser);

    public:
        virtual ~ForNode();
        virtual std::string name() const;

        ASTNode* startExpression() const;
        ASTNode* condition() const;
        ASTNode* loopExpression() const;
        bool evaluateConditionAtEnd() const;

        void codeGenCSource(CSourceContext& context);

    public:
        ASTNode* _startExpression;
        ASTNode* _condition;
        ASTNode* _loopExpression;
        ASTNode* _inExpression;
        bool _evaluateConditionAtEnd;
    };
}
