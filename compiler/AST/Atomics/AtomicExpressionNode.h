#pragma once

#include "AtomicNode.h"

namespace Three {
    class AtomicExpressionNode : public Three::AtomicNode {
    public:
        static AtomicExpressionNode* parse(Parser& parser, std::string type=std::string(""));
        std::string c11AtomicFunctionForOperator(const std::string& op);

    public:
        virtual std::string name() const;

        std::string type() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _type;
    };
}
