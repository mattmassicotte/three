#pragma once

#include "OperatorNode.h"

using namespace Language;

namespace Three {
    class MemberAccessNode : public OperatorNode {
    public:
        static MemberAccessNode* parse(Parser& parser, ASTNode* operand);
        
    public:
        virtual std::string name() const;

        std::string memberName() const;
        bool indirect() const;

        void codeGenCSource(CSourceContext& context);

    private:
        std::string _memberName;
        bool _indirect;
    };
}
