#pragma once

#include "../ASTNode.h"

namespace Three {
    class CaseNode : public ASTNode {
    public:
        static CaseNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        ASTNode* argument() const;

        void codeGen(CSourceContext& context);

    private:
        ASTNode* _argumentNode;
    };
}
