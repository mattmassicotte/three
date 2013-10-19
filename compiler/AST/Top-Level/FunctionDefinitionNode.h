#pragma once

#include "DefinitionNode.h"
#include "../../Constructs/Function.h"

namespace Three {
    class FunctionDefinitionNode : public DefinitionNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        Function* function() const;

        void codeGen(CSourceContext& context);

    private:
        Function* _function;
    };
}
