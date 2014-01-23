#pragma once

#include "AtomicNode.h"

namespace Three {
    class BarrierNode : public Three::AtomicNode {
    public:
        static BarrierNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        void codeGen(CSourceContext& context);
    };
}
