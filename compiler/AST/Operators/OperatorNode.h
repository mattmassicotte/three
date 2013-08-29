#pragma once

#include "../ASTNode.h"

namespace Language {
    class OperatorNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser, uint32_t precedence, ASTNode* leftOperand);
        static ASTNode* parseUnary(Parser& parser);

    private:
        static ASTNode* parseOperator(Parser& parser, ASTNode* leftOperand);

    public:
        virtual std::string name() const;

        std::string op() const;
        void        setOp(const std::string& string);
        bool        ternary() const;

        virtual void codeGenCSource(CSourceContext& context);

    private:
        std::string _operator;
    };
}
