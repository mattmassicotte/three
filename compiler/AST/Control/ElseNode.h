#pragma once

#include "../ASTNode.h"

namespace Three {
    class ElseNode : public ASTNode {
    public:
        static ElseNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);
    };
}
