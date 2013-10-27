#pragma once

#include "OperatorNode.h"

namespace Three {
    class FunctionCallOperatorNode : public OperatorNode {
    public:
        static FunctionCallOperatorNode* parse(Parser& parser, ASTNode* receiver);

    public:
        ASTNode* receiver() const;

        void codeGen(CSourceContext& context);

    private:
        bool receiverIsClosure() const;

    private:
        ASTNode* _receiver;
    };
}
