#pragma once

#include "../ASTNode.h"

namespace Three {
    class ElseNode : public ASTNode {
    public:
        static ElseNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        void codeGen(CSourceContext& context);
    };
}
