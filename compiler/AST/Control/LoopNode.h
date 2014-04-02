#pragma once

#include "../ASTNode.h"

namespace Three {
    class LoopNode : public ASTNode {
    public:
        static LoopNode* parse(NewParser& parser);

    public:
        virtual ~LoopNode();

        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        ASTNode* condition() const;
        bool evaluateConditionAtEnd() const;

    private:
        ASTNode* _condition;
        bool _evaluateConditionAtEnd;
    };
}
