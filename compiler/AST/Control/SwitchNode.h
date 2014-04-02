#pragma once

#include "../ASTNode.h"

namespace Three {
    class SwitchNode : public ASTNode {
    public:
        static SwitchNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        ASTNode* argument() const;
        ASTNode* elseNode() const;

    private:
        ASTNode* _argumentNode;
        ASTNode* _elseNode;
    };
}
