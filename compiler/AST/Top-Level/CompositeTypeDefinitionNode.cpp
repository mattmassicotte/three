#include "CompositeTypeDefinitionNode.h"
#include "compiler/Parser/Parser.h"
#include "compiler/Constructs/Scope.h"
#include "compiler/Constructs/Constant.h"
#include "../Variables/VariableDeclarationNode.h"
#include "StructureNode.h"
#include "EnumerationNode.h"
#include "UnionNode.h"
#include "CompositeTypeMemberNode.h"

namespace Three {
    CompositeTypeDefinitionNode* CompositeTypeDefinitionNode::parse(Parser& parser) {
        CompositeTypeDefinitionNode* node = nullptr;
        DataType::Kind typeKind;

        switch (parser.helper()->peek().type()) {
            case Token::Type::KeywordStructure:
                typeKind = DataType::Kind::Structure;
                node = new StructureNode();
                break;
            case Token::Type::KeywordEnumeration:
                typeKind = DataType::Kind::Enumeration;
                node = new EnumerationNode();
                break;
            case Token::Type::KeywordUnion:
                typeKind = DataType::Kind::Union;
                node = new UnionNode();
                break;
            default:
                assert(0 && "Message: Unexpected token type while parsing a composite type");
                break;
        }

        node->visibility = parser.context()->visibility();

        parser.helper()->next();

        if (parser.helper()->nextIf(":")) {
            node->setParsedIntegerSpecifier(parser, parser.parseIntegerSpecifier());
        }

        if (parser.helper()->peek().type() != Token::Type::Identifier) {
            assert(0 && "Message: Composite type name must start with an identifier");
        }

        // parse the name and create the type
        node->_name = parser.context()->scope()->qualifiedNameWithIdentifier(parser.helper()->nextStr());
        node->_definedType = DataType(typeKind);
        node->_definedType.setName(node->qualifiedName());

        if (parser.helper()->peek().type() == Token::Type::OperatorLessThan) {
            if (!parser.parseGenericParameters(node->_definedType)) {
                assert(0 && "Message: Unable to parse generic parameters");
            }
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: Composite type must be followed by a newline");
        }

        // define the type here, before parsing the internals, so we can
        // have recursive types
        parser.context()->defineTypeForName(node->_definedType, node->fullName());

        bool success = true;
        parser.helper()->parseUntilEnd([&] () {
            auto member = CompositeTypeMemberNode::parse(parser);

            if (!member) {
                success = false;
                return true;
            }

            // enumerations are special - they don't define subtypes
            // they define values
            if (typeKind == DataType::Kind::Enumeration) {
                if (!defineConstantForEnumerationMember(*node, *member, parser)) {
                    assert(0 && "Message: unable to define constants for enumeration");
                    success = false;
                    return true;
                }
            } else {
                node->_definedType.addSubtype(DataType::mutableVersion(member->dataType()));
            }

            return !node->addChild(member);
        });

        if (!success) {
            delete node;
            return nullptr;
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: end keyword must be followed by a newline");
        }

        // and now that are done, redefine the type, which will fill in the members
        parser.context()->redefineTypeForName(node->_definedType, node->fullName());

        return node;
    }

    bool CompositeTypeDefinitionNode::defineConstantForEnumerationMember(const CompositeTypeDefinitionNode& node, const CompositeTypeMemberNode& member, Parser& parser) {
        Constant* c = new Constant();

        c->type = parser.context()->dataTypeForName(QualifiedName("Int"));
        c->name = QualifiedName(member.name());
        c->name.prependName(parser.context()->scope()->fullNamespace());
        c->name.appendComponent(node._name.name());

        if (!parser.context()->defineConstant(c)) {
            delete c;
            return false;
        }

        return true;
    }

    std::string CompositeTypeDefinitionNode::name() const {
        return _name.name();
    }

    DataType CompositeTypeDefinitionNode::definedType() const {
        return _definedType;
    }

    std::string CompositeTypeDefinitionNode::fullName() const {
        return _name.to_s();
    }

    QualifiedName CompositeTypeDefinitionNode::qualifiedName() const {
        return _name;
    }
}
