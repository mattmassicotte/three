#pragma once

#include "../ASTNode.h"

namespace Three {
    class CaseNode : public ASTNode {
    public:
        static CaseNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        virtual std::string name() const;

        ASTNode* argument() const;

    private:
        ASTNode* _argumentNode;
    };
}
