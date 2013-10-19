#pragma once

#include "../ASTNode.h"

namespace Three {
    class BreakNode : public ASTNode {
    public:
        static BreakNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGen(CSourceContext& context);
    };
}
