#pragma once

#include "../ASTNode.h"

namespace Language {
    class SwitchNode : public ASTNode {
    public:
        static SwitchNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        ASTNode* argument() const;

        void codeGenCSource(CSourceContext& context);

    private:
        ASTNode* _argumentNode;
    };
}
