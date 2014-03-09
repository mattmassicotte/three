#pragma once

#include "OperatorNode.h"

namespace Three {
    class FunctionCallOperatorNode : public OperatorNode {
    public:
        static FunctionCallOperatorNode* parse(Parser& parser, ASTNode* receiver, ASTNode* firstArg=nullptr);

    public:
        void accept(ASTVisitor& visitor);

        ASTNode* receiver() const;
        bool receiverIsClosure() const;
        TypeReference receiverNodeType() const;

        void codeGen(CSourceContext& context);

    private:
        ASTNode* _receiver;
    };
}
