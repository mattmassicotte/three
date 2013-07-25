#pragma once

#include "DefinitionNode.h"
#include "../../Constructs/Function.h"

namespace Three {
    class FunctionDeclarationNode : public DefinitionNode {
    public:
        static FunctionDeclarationNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        Function* function() const;

        void codeGenCSource(CSourceContext& context);

    private:
        Function* _function;
    };
}
