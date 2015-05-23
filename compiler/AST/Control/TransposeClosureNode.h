#pragma once

#include "../ASTNode.h"

namespace Three {
    class TransposeClosureNode : public ASTNode {
    public:
        static TransposeClosureNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        virtual std::string name() const;
        virtual DataType dataType() const;
        void accept(ASTVisitor& visitor);
    };
}
