#pragma once

#include "../ASTNode.h"

namespace Three {
    class AtomicStatementNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        std::string transactionName() const;
        ASTNode* elseClause() const;

        ASTNode* lockFunction() const;
        ASTNode* unlockFunction() const;
        ASTNode* lockContext() const;

    private:
        ASTNode* _elseNode;
        std::string _transactionName;
        ASTNode* _lockFunctionNode;
        ASTNode* _unlockFunctionNode;
        ASTNode* _lockContextNode;
    };
}
