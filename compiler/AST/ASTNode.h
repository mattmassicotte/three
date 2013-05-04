#pragma once

#include <vector>

namespace Language {
    class ASTNode {
    public:
        ASTNode();
        virtual ~ASTNode();

        void     addChild(ASTNode* node);
        ASTNode* childAtIndex(unsigned int i) const;

    private:
        std::vector<ASTNode*> _childNodes;
    };
}
