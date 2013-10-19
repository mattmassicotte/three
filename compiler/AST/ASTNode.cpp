#include "ASTNode.h"
#include "../Constructs/Annotation.h"

#include <assert.h>
#include <sstream>

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

    TypeReference ASTNode::nodeType() const {
        return TypeReference();
    }

    std::vector<Annotation*> ASTNode::annotations() const {
        return _annotations;
    }

    void ASTNode::setAnnotations(const std::vector<Annotation*>& annotations) {
        _annotations = annotations;
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

    void ASTNode::codeGen(CSourceContext& context) {
    }

    void ASTNode::codeGenChildren(CSourceContext& context) {
        this->eachChildWithLast([=, &context] (ASTNode* node, bool last) {
            node->codeGen(context);
            if (node->statement()) {
                context.current()->printLine(";");
            }
        });
    }
}
