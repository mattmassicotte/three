#include "CompositeTypeDefinitionNode.h"
#include "compiler/Parser/Parser.h"
#include "../Variables/VariableDeclarationNode.h"
#include "StructureNode.h"
#include "EnumerationNode.h"
#include "UnionNode.h"

namespace Three {
    CompositeTypeDefinitionNode* CompositeTypeDefinitionNode::parse(Parser& parser) {
        CompositeTypeDefinitionNode* node = nullptr;
        NewDataType::Kind typeKind;

        switch (parser.helper()->peek().type()) {
            case Token::Type::KeywordStructure:
                typeKind = NewDataType::Kind::Structure;
                node = new StructureNode();
                break;
            case Token::Type::KeywordEnumeration:
                typeKind = NewDataType::Kind::Enumeration;
                node = new EnumerationNode();
                break;
            case Token::Type::KeywordUnion:
                typeKind = NewDataType::Kind::Union;
                node = new UnionNode();
                break;
            default:
                assert(0 && "Message: Unexpected token type while parsing a composite type");
                break;
        }

        parser.helper()->next();

        if (parser.helper()->nextIf(":")) {
            node->setParsedIntegerSpecifier(parser, parser.parseIntegerSpecifier());
        }

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: Composite type name must start with an identifier");
        }

        node->_name = parser.helper()->nextStr();
        node->visibility = parser.context()->visibility();

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: Composite type must be followed by a newline");
        }

        node->_definedType = NewDataType(typeKind);
        node->_definedType.setName(node->_name);

        // define the type here, before parsing the internals, so we can
        // have recursive types
        parser.context()->defineTypeForName(node->_definedType, node->_name);

        parser.helper()->parseUntilEnd([&] () {
            VariableDeclarationNode* member = VariableDeclarationNode::parse(parser, false);

            node->_definedType.addSubtype(member->dataType());
            if (!node->addChild(member)) {
                return true;
            }

            if (!parser.helper()->parseNewline()) {
                std::cout << parser.helper()->peek().str() << std::endl;
                assert(0 && "Message: composite type member must be followed by a newline");
            }

            return false;
        });

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: end keyword must be followed by a newline");
        }

        // and now that are done, redefine the type, which will fill in the members
        parser.context()->redefineTypeForName(node->_definedType, node->_name);

        return node;
    }

    std::string CompositeTypeDefinitionNode::name() const {
        return _name;
    }

    NewDataType CompositeTypeDefinitionNode::definedType() const {
        return _definedType;
    }
}
