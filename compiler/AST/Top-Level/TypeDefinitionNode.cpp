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
        // TODO: need to alias the type in the current translation unit

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
