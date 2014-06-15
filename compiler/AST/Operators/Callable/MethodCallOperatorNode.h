#pragma once

#include "CallableOperatorNode.h"

namespace Three {
    class MethodCallOperatorNode : public CallableOperatorNode {
    public:
        static MethodCallOperatorNode* parse(Parser& parser, ASTNode* receiver);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;

    private:
        std::string _name;
    };
}
