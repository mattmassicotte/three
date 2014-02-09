#pragma once

#include "../ASTNode.h"

namespace Three {
    class AbortStatementNode : public ASTNode {
    public:
        static AbortStatementNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        void codeGen(CSourceContext& context);
    };
}
