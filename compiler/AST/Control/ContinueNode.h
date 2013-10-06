#pragma once

#include "../ASTNode.h"

namespace Three {
    class ContinueNode : public ASTNode {
    public:
        static ContinueNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGen(CSourceContext& context);
    };
}
