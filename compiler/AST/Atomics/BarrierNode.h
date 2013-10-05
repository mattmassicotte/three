#pragma once

#include "AtomicNode.h"

namespace Language {
    class BarrierNode : public Three::AtomicNode {
    public:
        static BarrierNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

    public:
        void codeGenCSource(CSourceContext& context);
    };
}
