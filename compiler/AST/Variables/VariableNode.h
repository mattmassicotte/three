#pragma once

#include "../ASTNode.h"
#include "../../Constructs/Variable.h"

namespace Three {
    class VariableNode : public ASTNode {
    public:
        static VariableNode* parse(Parser& parser, const std::string& identifier);

    public:
        VariableNode();

        virtual TypeReference nodeType() const;
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        Variable* variable() const;
        void setVariable(Variable* var);
        bool closed() const;
        bool referenced() const;

        void codeGen(CSourceContext& context);

    private:
        Variable* _variable;
        bool      _referenced;
        bool      _closed;
    };
}
