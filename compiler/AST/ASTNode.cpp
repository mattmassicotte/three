#include "ASTNode.h"

#include <assert.h>
#include <sstream>

namespace Language {
    ASTNode::ASTNode() {
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

    uint32_t ASTNode::childCount() const {
        return _childNodes.size();
    }

    bool ASTNode::statement() const {
        return _statement;
    }

    void ASTNode::setStatement(bool value) {
        _statement = value;
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
}
