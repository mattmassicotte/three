#pragma once

#include "../ASTNode.h"
#include "../../Constructs/Variable.h"

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
        ASTNode* rangeStartExpression() const;
        ASTNode* rangeEndExpression() const;
        bool evaluateConditionAtEnd() const;

        Variable* rangeLoopVariable() const;

        void codeGen(CSourceContext& context);

    protected:
        void codeGenCondition(CSourceContext& context) const;
        void codeGenStartExpression(CSourceContext& context) const;
        void codeGenLoopExpression(CSourceContext& context) const;

    public:
        ASTNode* _startExpression;
        ASTNode* _condition;
        ASTNode* _loopExpression;
        ASTNode* _rangeStartExpression;
        ASTNode* _rangeEndExpression;
        bool _evaluateConditionAtEnd;
    };
}
