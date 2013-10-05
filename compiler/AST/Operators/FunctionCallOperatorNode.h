#pragma once

#include "OperatorNode.h"

using namespace Language;

namespace Three {
    class FunctionCallOperatorNode : public OperatorNode {
    public:
        static FunctionCallOperatorNode* parse(Parser& parser, ASTNode* receiver);

    public:
        ASTNode* receiver() const;

        void codeGenCSource(CSourceContext& context);

    private:
        ASTNode* _receiver;
    };
}
