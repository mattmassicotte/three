#pragma once

#include "../ASTNode.h"

using namespace Language;

namespace Three {
    class ContinueNode : public ASTNode {
    public:
        static ContinueNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
