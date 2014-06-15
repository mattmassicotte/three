#pragma once

#include "OperatorNode.h"

namespace Three {
    class MemberAccessNode : public OperatorNode {
    public:
        static MemberAccessNode* parse(Parser& parser, ASTNode* operand);
        
    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string name() const;

        std::string memberName() const;
        bool indirect() const;

    private:
        std::string _memberName;
        bool _indirect;
    };
}
