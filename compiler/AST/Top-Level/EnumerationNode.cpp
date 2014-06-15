#include "EnumerationNode.h"
#include "../Variables/VariableDeclarationNode.h"
#include "compiler/Parser/Parser.h"

#include <assert.h>

namespace Three {
    std::string EnumerationNode::nodeName() const {
        return "Enumeration";
    }

    void EnumerationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::vector<std::string> EnumerationNode::identifiers() const {
        return _identifiers;
    }

    uint32_t EnumerationNode::size() const {
        return _size;
    }

    void EnumerationNode::eachMemberWithLast(std::function<void (const std::string&, bool)> func) {
        this->eachChildWithLast([&] (ASTNode* child, bool last) {
            VariableDeclarationNode* member = dynamic_cast<VariableDeclarationNode*>(child);

            assert(!member->initializerExpression());

            func(member->name(), last);
        });
    }

    void EnumerationNode::setParsedIntegerSpecifier(Parser& parser, uint32_t value) {
        _size = value;
    }
}
