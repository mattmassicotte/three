#pragma once

#include "../ASTNode.h"

namespace Three {
    class OperatorNode : public ASTNode {
    public:
        static ASTNode* parse(Parser& parser, ASTNode* left, uint32_t precedence);
        static ASTNode* parseUnary(Parser& parser);
        static ASTNode* parseTailing(Parser& parser, ASTNode* leftNode);
        static ASTNode* parseSingleTailing(Parser& parser, ASTNode* leftNode);

    private:
        static ASTNode* parseOperator(Parser& parser, ASTNode* leftOperand);

    public:
        virtual TypeReference nodeType() const;
        virtual std::string name() const;

        virtual std::string str() const;

        std::string op() const;
        void        setOp(const std::string& string);
        bool        ternary() const;

        virtual void codeGenCSource(CSourceContext& context);

    private:
        std::string _operator;
    };
}
