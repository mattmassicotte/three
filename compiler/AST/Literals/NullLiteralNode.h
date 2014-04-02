#pragma once

#include "LiteralNode.h"

namespace Three {
    class NullLiteralNode : public LiteralNode {
    public:
        static NullLiteralNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
