#include "ASTNode.h"

#include <assert.h>

namespace Language {
    ASTNode::ASTNode() {
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

    ASTNode* ASTNode::childAtIndex(unsigned int i) const {
        return _childNodes.at(i);
    }
}
