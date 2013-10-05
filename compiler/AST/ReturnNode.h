#pragma once

#include "ASTNode.h"

namespace Language {
    class ReturnNode : public ASTNode {
    public:
        static ReturnNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);

    private:
        bool _endsTransaction;
    };
}
