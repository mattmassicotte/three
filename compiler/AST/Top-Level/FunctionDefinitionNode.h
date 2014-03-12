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
        void accept(ASTVisitor& visitor);

        Function* function() const;
        ASTNode* ensureClause() const;

        void codeGen(CSourceContext& context);

    private:
        Function* _function;
        ASTNode* _ensureBlock;
    };
}
