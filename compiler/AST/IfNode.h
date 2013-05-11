#pragma once

#include "ASTNode.h"
#include "ElseNode.h"

namespace Language {
    class IfNode : public ASTNode {
    public:
        static IfNode* parse(Parser& parser);

    public:
        IfNode();

        virtual std::string name() const;

        ASTNode* condition() const;
        ElseNode* elseStatement() const;

        void codeGenCSource(CSourceContext& context);

    private:
        ASTNode*  _conditionNode;
        ElseNode* _elseNode;
    };
}
