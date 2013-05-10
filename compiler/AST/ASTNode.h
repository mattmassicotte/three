#pragma once

#include <sstream>
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
        ASTNode* childAtIndex(uint32_t i) const;
        uint32_t childCount() const;

        bool statement() const;
        void setStatement(bool value);

        virtual std::string name() const = 0;
        virtual std::string str() const;
        std::string recursiveStr(uint32_t depth = 0) const;

        virtual void renderCCode(std::stringstream& stream) = 0;

    private:
        std::vector<ASTNode*> _childNodes;
        bool                  _statement;
    };
}
