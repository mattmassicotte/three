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
        parser.parseNewline();

        // Here's an interesting problem.  If the struct refers to itself in the 
        // definition, it has to be already defined.  Additionally, in C, that's
        // not possible without referencing a specially-named version.
        DataType* type = new DataType(DataType::Flavor::Structure, node->_name);

        type->setCSourcePrependStructKeyword(true);

        for (Annotation* annotation : parser.currentScope()->annotations()) {
            annotation->applyToDataType(type);
        }
        parser.currentScope()->clearAnnotations();

        parser.currentModule()->addDataType(type);

        // parse the members, but be sure not to create scoped variables for them
        parser.parseUntilEnd([&] () {
            node->addChild(VariableDeclarationNode::parse(parser, false));
            parser.parseNewline();
        });

        parser.parseNewline();

        // now that we are done, there's no possibility of self-references so we can
        // get rid of the "struct" keyword on the type
        
        type->setCSourcePrependStructKeyword(false);

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

        context.current()->printLineAndIndent("typedef struct _" + this->structureName() + " {");

        this->codeGenCSourceForChildren(context);

        context.current()->outdentAndPrintLine("} " + this->structureName() + ";");

        this->renderCSourcePackingStop(context);

        context.setCurrent(CSourceContext::Section::Body);
    }
}
