#include "StructureNode.h"
#include "../VariableDeclarationNode.h"
#include "../../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    StructureNode* StructureNode::parse(Parser& parser) {
        StructureNode* node = new StructureNode();

        assert(parser.next().type() == Token::Type::KeywordStructure);

        node->_packing = 0;
        if (parser.nextIf(":")) {
            // TODO: this needs to be more strict about the input types
            assert(parser.peek().type() == Token::Type::NumericLiteral);

            node->_packing = strtol(parser.next().str().c_str(), NULL, 10);
        }

        assert(parser.peek().type() == Token::Type::Identifier);
        node->_name = parser.next().str();
        parser.parseNewline(true);

        // We need to define our type here, in case it is used recursively
        DataType* type = new DataType(DataType::Flavor::Structure, node->_name);
        parser.currentModule()->addDataType(type);

        parser.parseUntilEnd([&] () {
            node->addChild(VariableDeclarationNode::parse(parser));
            parser.parseNewline(true);
        });

        parser.parseNewline(true);

        // here, we need to iterate through the elements of the structure, and fill in
        // the actual members
        // TODO

        return node;
    }

    std::string StructureNode::name() const {
        return "Structure";
    }

    uint32_t StructureNode::packing() const {
        return _packing;
    }

    std::string StructureNode::structureName() const {
        return _name;
    }

    void StructureNode::renderCSourcePackingStart(CSourceContext& context) {
        if (this->packing() == 0) {
            return;
        }

        std::stringstream s;

        // TODO: this is kinda stupid just to convert into a string...
        s << this->packing();

        context.declarations()->printLine("#pragma pack(push)");
        context.declarations()->printLine("#pragma pack(" + s.str() + ")");
    }

    void StructureNode::renderCSourcePackingStop(CSourceContext& context) {
        if (this->packing() == 0) {
            return;
        }

        context.declarations()->printLine("#pragma pack(pop)");
    }

    void StructureNode::codeGenCSource(CSourceContext& context) {
        context.setCurrent(CSourceContext::Section::Declarations);

        this->renderCSourcePackingStart(context);

        context.current()->printLineAndIndent("typedef struct {");

        this->codeGenCSourceForChildren(context);

        context.current()->outdentAndPrintLine("} " + this->structureName() + ";");

        this->renderCSourcePackingStop(context);

        context.setCurrent(CSourceContext::Section::Body);
    }
}
