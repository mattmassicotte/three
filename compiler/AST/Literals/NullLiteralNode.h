#pragma once

#include "../ASTNode.h"

namespace Three {
    class NullLiteralNode : public ASTNode {
    public:
        static NullLiteralNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        void codeGen(CSourceContext& context);
    };
}
