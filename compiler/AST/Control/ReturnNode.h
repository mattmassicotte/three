#pragma once

#include "../ASTNode.h"

namespace Three {
    class ReturnNode : public ASTNode {
    public:
        static ReturnNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        bool endsTransaction() const;

        void codeGen(CSourceContext& context);

    private:
        bool _endsTransaction;
    };
}
