#pragma once

#include "../ASTNode.h"

namespace Three {
    class AtomicStatementNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGen(CSourceContext& context);

    protected:
        void codeGenTransactionAllocation(CSourceContext& context);

    private:
        ASTNode* _elseNode;
        std::string _transactionName;
    };
}
