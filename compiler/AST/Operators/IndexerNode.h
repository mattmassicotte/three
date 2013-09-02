#pragma once

#include "OperatorNode.h"

using namespace Language;

namespace Three {
    class IndexerNode : public OperatorNode {
    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
