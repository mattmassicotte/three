#pragma once

#include "../ASTNode.h"

namespace Language {
    class LoopNode : public ASTNode {
    public:
        static LoopNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
