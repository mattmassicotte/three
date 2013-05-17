#pragma once

#include "ASTNode.h"
#include "../Constructs/Function.h"

namespace Language {
    class FunctionDefinitionNode : public ASTNode {
    public:
        static FunctionDefinitionNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        Function* function() const;

        void codeGenCSource(CSourceContext& context);

    private:
        Function* _function;
    };
}
