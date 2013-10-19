#pragma once

#include "AtomicNode.h"

namespace Three {
    class OperatorNode;
}

namespace Three {
    class AtomicExpressionNode : public Three::AtomicNode {
    public:
        static AtomicExpressionNode* parse(Parser& parser, bool statement);
        std::string c11AtomicFunctionForOperator(const std::string& op);

    public:
        virtual std::string name() const;

        void codeGen(CSourceContext& context);

    private:
        void codeGenAtomicVariable(CSourceContext& context, OperatorNode* op);
        std::string c11AtomicFunctionForFullOperation(const std::string& op);
        bool c11AtomicFunctionIsLoadOperation(const std::string& op);
    };
}
