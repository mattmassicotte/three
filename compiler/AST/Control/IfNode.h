#pragma once

#include "../ASTNode.h"
#include "ElseNode.h"

namespace Three {
    class IfNode : public ASTNode {
    public:
        static IfNode* parse(Parser& parser);
        static ASTNode* parseTailing(Parser& parser, ASTNode* node);

    public:
        IfNode();

        std::string nodeName() const;
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        ASTNode* condition() const;
        void     setCondition(ASTNode* node);
        ASTNode* elseStatement() const;

    private:
        ASTNode* _conditionNode;
        ASTNode* _elseNode;
    };
}
