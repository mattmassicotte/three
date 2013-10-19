#pragma once

#include "../ASTNode.h"

namespace Three {
    class ReturnNode : public ASTNode {
    public:
        static ReturnNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGen(CSourceContext& context);

    private:
        bool _endsTransaction;
    };
}
