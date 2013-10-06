#pragma once

#include "../ASTNode.h"

namespace Three {
    class ElseNode : public ASTNode {
    public:
        static ElseNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGen(CSourceContext& context);
    };
}
