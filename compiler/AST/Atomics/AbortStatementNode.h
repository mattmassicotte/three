#pragma once

#include "../ASTNode.h"

namespace Three {
    class AbortStatementNode : public ASTNode {
    public:
        static AbortStatementNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        std::string transactionName() const;

    private:
        std::string _transactionName;
    };
}
