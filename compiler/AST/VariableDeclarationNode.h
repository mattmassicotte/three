#pragma once

#include "ASTNode.h"
#include "../Constructs/Variable.h"

namespace Language {
    class VariableDeclarationNode : public ASTNode {
    public:
        static VariableDeclarationNode* parse(Parser& parser, bool createVariable=true);

    public:
        virtual ~VariableDeclarationNode();

        virtual std::string name() const;
        Variable* variable() const;
        ASTNode* initializerExpression() const;

        void codeGenCSource(CSourceContext& context);

    private:
        Variable* _variable;
        ASTNode* _initializerExpression;
    };
}
