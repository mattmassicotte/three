#pragma once

#include "../ASTNode.h"
#include "../../Constructs/Function.h"
#include "../../Constructs/Variable.h"

namespace Three {
    class ClosureNode : public ASTNode {
    public:
        static ClosureNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        std::vector<Variable*> capturedVariables() const;
        std::vector<Variable*> referencedVariables() const;
        void eachCapturedVariable(std::function<void (Variable*, bool, bool)> func) const;

        void codeGen(CSourceContext& context);

    private:
        void codeGenEnvironmentStructure(CSourceContext& context) const;
        void codeGenBodyFunction(CSourceContext& context);
        void codeGenEnvironmentCapture(CSourceContext& context) const;

    private:
        Function*   _function;
        TypeReference _type;
        std::string _name;
        std::string _environmentName;
        std::vector<Variable*> _referencedVariables;
        std::vector<Variable*> _capturedVariables;

        uint32_t _savedCSourceLocation;
    };
}
