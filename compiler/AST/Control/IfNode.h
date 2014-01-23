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

        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        ASTNode* condition() const;
        void     setCondition(ASTNode* node);
        ElseNode* elseStatement() const;

        void codeGen(CSourceContext& context);

    private:
        ASTNode*  _conditionNode;
        ElseNode* _elseNode;
    };
}
