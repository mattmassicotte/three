#pragma once

#include "AtomicNode.h"

namespace Three {
    class OperatorNode;

    class AtomicExpressionNode : public Three::AtomicNode {
    public:
        static AtomicExpressionNode* parse(Parser& parser);
        std::string c11AtomicFunctionForOperator(const std::string& op);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        OperatorNode* op() const;

    private:
        std::string c11AtomicFunctionForFullOperation(const std::string& op);
        bool c11AtomicFunctionIsLoadOperation(const std::string& op);
    };
}
