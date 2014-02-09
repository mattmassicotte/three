#pragma once

#include "../ASTNode.h"

namespace Three {
    class AtomicStatementNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        std::string transactionName() const;
        ASTNode* elseClause() const;

        void codeGen(CSourceContext& context);

    protected:
        void codeGenTransactionAllocation(CSourceContext& context);

    private:
        ASTNode* _elseNode;
        std::string _transactionName;
    };
}
