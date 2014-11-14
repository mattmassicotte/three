#pragma once

#include "../CodeGen/CSourceContext.h"
#include "ASTVisitor.h"

#include <sstream>
#include <string>
#include <vector>
#include <functional>

// forward declarations
namespace Three {
    class Parser;
    class DataType;
};

namespace Three {
    class ASTNode {
    public:
        ASTNode();
        ASTNode(const ASTNode& other) = delete;
        virtual ~ASTNode();

        bool     addChild(ASTNode* node);
        ASTNode* childAtIndex(uint32_t i) const;
        ASTNode* lastChild() const;
        uint32_t childCount() const;
        void     eachChild(std::function<void (ASTNode*, uint32_t)> func);
        void     eachChildWithLast(std::function<void (ASTNode*, bool)> func);

        bool statement() const;
        void setStatement(bool value);
        virtual bool includesParentheses() const;
        virtual DataType dataType() const;

        virtual std::string name() const = 0;
        virtual std::string nodeName() const;
        virtual std::string str() const;
        std::string recursiveStr(uint32_t depth = 0) const;

        virtual void accept(ASTVisitor& visitor);
        void acceptChildren(ASTVisitor& visitor);

    private:
        std::vector<ASTNode*> _childNodes;
        bool _statement;
    };
}
