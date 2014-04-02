#include "ASTNode.h"
#include "compiler/constructs/NewDataType.h"

#include <assert.h>
#include <sstream>
#include <iostream>

namespace Three {
    ASTNode::ASTNode() : _statement(false) {
        _childNodes.clear();
    }

    ASTNode::~ASTNode() {
        for(ASTNode* node : _childNodes) {
            delete node;
        }
    }

    void ASTNode::addChild(ASTNode* node) {
        assert(node && "argument cannot be NULL");

        _childNodes.push_back(node);
    }

    ASTNode* ASTNode::childAtIndex(uint32_t i) const {
        return _childNodes.at(i);
    }

    ASTNode* ASTNode::lastChild() const {
        if (this->childCount() == 0) {
            return nullptr;
        }

        return _childNodes.back();
    }
    uint32_t ASTNode::childCount() const {
        return _childNodes.size();
    }

    void ASTNode::eachChild(std::function<void (ASTNode*, uint32_t)> func) {
        for (uint32_t i = 0; i < this->childCount(); ++i) {
            func(this->childAtIndex(i), i);
        }
    }

    void ASTNode::eachChildWithLast(std::function<void (ASTNode*, bool)> func) {
        uint32_t count = this->childCount();
        
        for (uint32_t i = 0; i < count; ++i) {
            func(this->childAtIndex(i), i == (count - 1));
        }
    }

    bool ASTNode::statement() const {
        return _statement;
    }

    void ASTNode::setStatement(bool value) {
        _statement = value;
    }

    bool ASTNode::includesParentheses() const {
        return false;
    }

    NewDataType ASTNode::dataType() const {
        return NewDataType();
    }

    std::string ASTNode::nodeName() const {
        return this->name();
    }

    std::string ASTNode::str() const {
        return this->name();
    }

    std::string ASTNode::recursiveStr(uint32_t depth) const {
        std::stringstream s;

        // padding
        for (int i = 0; i < depth; ++i) {
            s << "  ";
        }

        // node-specific string representation
        s << this->str();

        // and then finally the recursion across all child nodes
        for (ASTNode* node : _childNodes) {
            s << std::endl;
            s << node->recursiveStr(depth+1);
        }

        return s.str();
    }

    void ASTNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void ASTNode::acceptChildren(ASTVisitor& visitor) {
        this->eachChild([&] (ASTNode* child, uint32_t _) {
            child->accept(visitor);
        });
    }
}
