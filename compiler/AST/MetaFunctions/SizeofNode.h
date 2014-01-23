#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TypeReference.h"

namespace Three {
    class SizeofNode : public ASTNode {
    public:
        static SizeofNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        TypeReference argument() const;

        void codeGen(CSourceContext& context);

    private:
        TypeReference _typeArgument;
    };
}
