#pragma once

#include "../ASTNode.h"

namespace Three {
    class OperatorNode : public ASTNode {
    public:
        static ASTNode* parse(NewParser& parser, ASTNode* left, uint32_t precedence);
        static ASTNode* parseUnary(NewParser& parser);
        static ASTNode* parseTailing(NewParser& parser, ASTNode* leftNode);
        static ASTNode* parseSingleTailing(NewParser& parser, ASTNode* leftNode);

    private:
        static OperatorNode* createOperator(NewParser& parser, bool unary = false);

    public:
        NewDataType dataType() const;
        virtual std::string nodeName() const;
        virtual std::string name() const;

        virtual std::string str() const;

        std::string op() const;
        void        setOp(const std::string& string);
        bool        ternary() const;

    private:
        std::string _operator;
    };
}
