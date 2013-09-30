#pragma once

#include "ASTNode.h"
#include "../Constructs/Variable.h"

namespace Language {
    class VariableNode : public ASTNode {
    public:
        static VariableNode* parse(Parser& parser);

    public:
        VariableNode();

        virtual TypeReference nodeType() const;

        virtual std::string name() const;
        Variable* variable() const;
        void setVariable(Variable* var);

        void codeGenCSource(CSourceContext& context);

    private:
        Variable* _variable;
        bool      _referenced;
        bool      _closed;
    };
}
