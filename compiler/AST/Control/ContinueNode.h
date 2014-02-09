#pragma once

#include "../ASTNode.h"

namespace Three {
    class ContinueNode : public ASTNode {
    public:
        static ContinueNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        void codeGen(CSourceContext& context);
    };
}
