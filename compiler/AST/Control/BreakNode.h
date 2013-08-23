#pragma once

#include "../ASTNode.h"

using namespace Language;

namespace Three {
    class BreakNode : public ASTNode {
    public:
        static BreakNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
