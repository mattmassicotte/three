#pragma once

#include "ASTNode.h"
#include "../Constructs/Variable.h"

namespace Language {
    class VariableDeclarationNode : public ASTNode {
    public:
        static VariableDeclarationNode* parse(Parser& parser, bool createVariable=true);

    public:
        virtual std::string name() const;
        Variable* variable() const;

        void codeGenCSource(CSourceContext& context);

    private:
        Variable* _variable;
    };
}
