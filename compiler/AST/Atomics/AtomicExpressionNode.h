#pragma once

#include "../ASTNode.h"

namespace Language {
    class AtomicExpressionNode : public ASTNode {
    public:
        static AtomicExpressionNode* parse(Parser& parser, std::string type=std::string(""));

    public:
        virtual std::string name() const;

        std::string type() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _type;
    };
}
