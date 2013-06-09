#pragma once

#include "../ASTNode.h"

namespace Language {
    class CaseNode : public ASTNode {
    public:
        static CaseNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        ASTNode* argument() const;

        void codeGenCSource(CSourceContext& context);

    private:
        ASTNode* _argumentNode;
    };
}
