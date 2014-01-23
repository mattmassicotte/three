#pragma once

#include "AtomicNode.h"

namespace Three {
    class AtomicExpressionNode : public Three::AtomicNode {
    public:
        static AtomicExpressionNode* parse(Parser& parser, bool statement);
        std::string c11AtomicFunctionForOperator(const std::string& op);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        class OperatorNode* op() const;

        void codeGen(CSourceContext& context);

    private:
        void codeGenAtomicVariable(CSourceContext& context, class OperatorNode* op);
        std::string c11AtomicFunctionForFullOperation(const std::string& op);
        bool c11AtomicFunctionIsLoadOperation(const std::string& op);
    };
}
