#pragma once

#include "CallableOperatorNode.h"

namespace Three {
    class FunctionCallOperatorNode : public CallableOperatorNode {
    public:
        static FunctionCallOperatorNode* parse(Parser& parser, ASTNode* receiver, ASTNode* firstArg=nullptr);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
