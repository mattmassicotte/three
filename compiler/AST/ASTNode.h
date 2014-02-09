#pragma once

#include "../CodeGen/CSourceContext.h"
#include "../Constructs/TypeReference.h"
#include "ASTVisitor.h"

#include <sstream>
#include <string>
#include <vector>
#include <functional>

// forward declaration
namespace Three {
    class Parser;
    class Annotation;
};

namespace Three {
    class ASTNode {
    public:
        ASTNode();
        ASTNode(const ASTNode& other) = delete;
        virtual ~ASTNode();

        void     addChild(ASTNode* node);
        ASTNode* childAtIndex(uint32_t i) const;
        uint32_t childCount() const;
        void     eachChild(std::function<void (ASTNode*, uint32_t)> func);
        void     eachChildWithLast(std::function<void (ASTNode*, bool)> func);

        bool statement() const;
        void setStatement(bool value);
        virtual bool includesParentheses() const;
        virtual TypeReference nodeType() const;

        std::vector<Annotation*> annotations() const;
        void setAnnotations(const std::vector<Annotation*>& annotations);

        virtual std::string name() const = 0;
        std::string nodeName() const;
        virtual std::string str() const;
        std::string recursiveStr(uint32_t depth = 0) const;

        virtual void accept(ASTVisitor& visitor);
        void acceptChildren(ASTVisitor& visitor);

        virtual void codeGen(CSourceContext& context);
        void codeGenChildren(CSourceContext& context);

    private:
        std::vector<ASTNode*>    _childNodes;
        bool                     _statement;
        std::vector<Annotation*> _annotations;
    };
}
