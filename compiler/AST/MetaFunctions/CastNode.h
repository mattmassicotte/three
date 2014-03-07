#pragma once

#include "../ASTNode.h"
#include "../../Constructs/TypeReference.h"

namespace Three {
    class CastNode : public ASTNode {
    public:
        static CastNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        TypeReference argument() const;

    private:
        TypeReference _typeArgument;
    };
}
