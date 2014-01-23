#pragma once

#include "../ASTNode.h"

namespace Three {
    class SwitchNode : public ASTNode {
    public:
        static SwitchNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        ASTNode* argument() const;
        ASTNode* elseNode() const;

        void codeGen(CSourceContext& context);

    private:
        ASTNode* _argumentNode;
        ASTNode* _elseNode;
    };
}
