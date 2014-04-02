#include "StructureNode.h"
#include "../Variables/VariableDeclarationNode.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>
#include <sstream>

namespace Three {
    std::string StructureNode::nodeName() const {
        return "Structure Definition";
    }

    void StructureNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    uint32_t StructureNode::packing() const {
        return _packing;
    }

    void StructureNode::setParsedIntegerSpecifier(NewParser& parser, uint32_t value) {
        _packing = value;
    }
}
