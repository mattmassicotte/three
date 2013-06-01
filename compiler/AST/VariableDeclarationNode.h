#pragma once

#include "ASTNode.h"
#include "../Constructs/Variable.h"

namespace Language {
    class VariableDeclarationNode : public ASTNode {
    public:
        static VariableDeclarationNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);

    private:
        Variable* _variable;
    };
}
