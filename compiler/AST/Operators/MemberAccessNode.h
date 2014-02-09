#pragma once

#include "OperatorNode.h"

namespace Three {
    class MemberAccessNode : public OperatorNode {
    public:
        static MemberAccessNode* parse(Parser& parser, ASTNode* operand);
        
    public:
        TypeReference nodeType() const;

        std::string name() const;
        void accept(ASTVisitor& visitor);

        std::string memberName() const;
        bool indirect() const;

        void codeGen(CSourceContext& context);

    private:
        std::string _memberName;
        bool _indirect;
    };
}
