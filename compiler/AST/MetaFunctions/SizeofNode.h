#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class SizeofNode : public MetafunctionNode {
    public:
        static SizeofNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
