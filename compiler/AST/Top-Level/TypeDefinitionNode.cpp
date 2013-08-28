#include "TypeDefinitionNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    TypeDefinitionNode* TypeDefinitionNode::parse(Parser& parser) {
        TypeDefinitionNode* node = new TypeDefinitionNode();

        // def:type NewName ExistingType
        assert(parser.peek().type() == Token::Type::Identifier);
        node->_destTypeName = parser.next().str();

        node->_sourceType = parser.parseType();

        parser.parseNewline();

        // TODO: This isn't quite right.  The new type needs to include information about the reference level.
        parser.currentModule()->addDataType(node->_destTypeName, node->_sourceType.referencedType());

        return node;
    }

    std::string TypeDefinitionNode::name() const {
        return "TypeDefinition";
    }

    TypeReference TypeDefinitionNode::sourceType() const {
        return _sourceType;
    }

    std::string TypeDefinitionNode::destTypeName() const {
        return _destTypeName;
    }

    void TypeDefinitionNode::codeGenCSource(CSourceContext& context) {
    }
}
