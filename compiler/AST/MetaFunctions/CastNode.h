#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class CastNode : public MetafunctionNode {
    public:
        static CastNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
