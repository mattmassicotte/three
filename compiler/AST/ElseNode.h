#pragma once

#include "ASTNode.h"

namespace Language {
    class ElseNode : public ASTNode {
    public:
        static ElseNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
