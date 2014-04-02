#pragma once

#include "../ASTNode.h"

namespace Three {
    class ContinueNode : public ASTNode {
    public:
        static ContinueNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
