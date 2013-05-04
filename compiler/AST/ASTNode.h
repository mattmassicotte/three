#pragma once

#include <string>
#include <vector>

// forward declaration
namespace Language {
    class Parser;
};

namespace Language {
    class ASTNode {
    public:
        ASTNode();
        ASTNode(const ASTNode& other) = delete;
        virtual ~ASTNode();

        void     addChild(ASTNode* node);
        ASTNode* childAtIndex(unsigned int i) const;

        virtual std::string name() const = 0;
        virtual std::string str() const;
        std::string recursiveStr(unsigned int depth = 0) const;

    private:
        std::vector<ASTNode*> _childNodes;
    };
}
