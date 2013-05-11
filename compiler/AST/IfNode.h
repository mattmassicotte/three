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

        virtual void renderCCode(std::stringstream& stream, uint32_t indentation);

    private:
        ASTNode*  _conditionNode;
        ElseNode* _elseNode;
    };
}
