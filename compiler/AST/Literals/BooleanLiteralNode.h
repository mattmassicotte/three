#pragma once

#include "../ASTNode.h"

namespace Three {
    class BooleanLiteralNode : public ASTNode {
    public:
        static BooleanLiteralNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);
        TypeReference nodeType() const;

        bool value() const;

    private:
        bool _value;
        TypeReference _type;
    };
}
