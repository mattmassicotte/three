#include "EnumerationNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    EnumerationNode* EnumerationNode::parse(Parser& parser) {
        EnumerationNode* node = new EnumerationNode();

        assert(parser.next().type() == Token::Type::KeywordEnumeration);
        assert(parser.peek().type() == Token::Type::Identifier);

        node->_name = parser.next().str();
        node->setVisibility(parser.context()->visibility());

        parser.parseNewline();

        std::string identifierPrefix = parser.currentScope()->fullNamespace();
        if (identifierPrefix.length() > 0) {
            identifierPrefix += "_3_";
        }
        identifierPrefix += node->_name + "_3_";

        parser.parseUntilEnd([&] () {
            assert(parser.peek().type() == Token::Type::Identifier);

            std::string identifier = parser.next().str();
            node->_identifiers.push_back(identifier);

            if (parser.nextIf("=")) {
                ASTNode* expression = parser.parseExpression();
            }

            parser.parseNewline();

            parser.currentModule()->addConstant(identifierPrefix + identifier, identifier);
        });

        parser.parseNewline();

        DataType* type = new DataType(DataType::Flavor::Enumeration, node->_name);
        type->setNamespace(parser.currentScope()->fullNamespace());

        node->_type = type;

        parser.currentModule()->addDataType(type);

        return node;
    }

    std::string EnumerationNode::name() const {
        return "Enumeration";
    }

    void EnumerationNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string EnumerationNode::structureName() const {
        return _name;
    }

    std::vector<std::string> EnumerationNode::identifiers() const {
        return _identifiers;
    }

    DataType* EnumerationNode::type() const {
        return _type;
    }

    void EnumerationNode::codeGen(CSourceContext& context) {
        context.adjustCurrentForVisibility(this->visibility(), [&] (CSource* source) {
            source->printLineAndIndent("enum {");

            for (uint32_t i = 0; i < this->_identifiers.size(); ++i) {
                *source << this->_type->fullyQualifiedName() << "_3_" << this->_identifiers.at(i);

                if (i < this->_identifiers.size() - 1) {
                    *source << ",";
                }
                source->printLine("");
            }

            source->outdentAndPrintLine("};");
            *source << "typedef uint32_t " << this->_type->fullyQualifiedName();
            source->printLine(";");
            source->printLine("");
        });
    }
}
