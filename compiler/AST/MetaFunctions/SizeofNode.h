#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TypeReference.h"

namespace Three {
    class SizeofNode : public ASTNode {
    public:
        static SizeofNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void codeGenCSource(CSourceContext& context);

    private:
        TypeReference _typeArgument;
    };
}
