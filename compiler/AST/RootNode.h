#pragma once

#include "ASTNode.h"

namespace Language {
    class RootNode : public ASTNode {
    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);
    };
}
