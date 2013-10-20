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

        parser.parseUntilEnd([&] () {
            assert(parser.peek().type() == Token::Type::Identifier);

            std::string identifier = parser.next().str();
            node->_identifiers.push_back(identifier);

            if (parser.nextIf("=")) {
                ASTNode* expression = parser.parseExpression();
            }

            parser.parseNewline();

            // TODO: the constant needs to actually be defined here
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

    std::string EnumerationNode::structureName() const {
        return _name;
    }

    void EnumerationNode::codeGen(CSourceContext& context) {
        context.adjustCurrentForVisibility(this->visibility(), [&] (CSource* source) {
            *source << "typedef uint32_t " << this->_type->fullyQualifiedName();
            source->printLine(";");

            for (uint32_t i = 0; i < this->_identifiers.size(); ++i) {
                *source << "#define " << this->_identifiers.at(i) << " ((uint32_t)" << std::to_string(i);
                source->printLine(")");
            }

            source->printLine("");
        });
    }
    
    //     context.current()->printLineAndIndent("typedef enum {");
    // 
    //     for (uint32_t i = 0; i < _identifiers.size(); ++i) {
    //         context.current()->printLine(_identifiers.at(i));
    //     }
    // 
    //     context.current()->outdentAndPrintLine("} " + this->structureName() + ";");
    // 
    //     context.setCurrent(CSourceContext::Section::Body);
    // }
}
