#pragma once

#include "../ASTNode.h"
#include "compiler/constructs/NewVariable.h"

namespace Three {
    class ForNode : public ASTNode {
    public:
        static ForNode* parse(Parser& parser);

    public:
        virtual ~ForNode();
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        ASTNode* startExpression() const;
        ASTNode* condition() const;
        ASTNode* loopExpression() const;
        ASTNode* rangeStartExpression() const;
        ASTNode* rangeEndExpression() const;

        NewVariable* rangeLoopVariable() const;

    public:
        ASTNode* _startExpression;
        ASTNode* _condition;
        ASTNode* _loopExpression;
        ASTNode* _rangeStartExpression;
        ASTNode* _rangeEndExpression;
    };
}
