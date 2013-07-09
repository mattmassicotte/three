#pragma once

#include "../ASTNode.h"

namespace Language {
    class NullLiteralNode : public ASTNode {
    public:
        static NullLiteralNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
