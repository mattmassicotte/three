#pragma once

#include "LiteralNode.h"

namespace Three {
    class BooleanLiteralNode : public LiteralNode {
    public:
        static BooleanLiteralNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        bool value() const;

    private:
        bool _value;
    };
}
