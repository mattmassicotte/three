#pragma once

#include "../ASTNode.h"
#include "../../Constructs/Variable.h"

namespace Three {
    class VariableDeclarationNode : public ASTNode {
    public:
        static VariableDeclarationNode* parse(Parser& parser, bool createVariable=true);

    public:
        virtual ~VariableDeclarationNode();

        virtual std::string name() const;
        TypeReference nodeType() const;
        void accept(ASTVisitor& visitor);

        Variable* variable() const;
        ASTNode* initializerExpression() const;

        void codeGen(CSourceContext& context);

        bool global;

    private:
        Variable* _variable;
        ASTNode* _initializerExpression;
    };
}
