#pragma once

#include "../ASTNode.h"

namespace Three {
    class ReturnNode : public ASTNode {
    public:
        static ReturnNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        bool endsTransaction() const;
        std::string transactionName() const;

    private:
        bool _endsTransaction;
        std::string _transactionName;
    };
}
