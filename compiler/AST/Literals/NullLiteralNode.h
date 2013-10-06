#pragma once

#include "../ASTNode.h"

namespace Three {
    class NullLiteralNode : public ASTNode {
    public:
        static NullLiteralNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
