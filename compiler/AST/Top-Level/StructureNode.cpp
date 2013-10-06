#include "StructureNode.h"
#include "../Variables/VariableDeclarationNode.h"
#include "../../Parser.h"

#include <assert.h>
#include <sstream>

namespace Three {
    StructureNode* StructureNode::parse(Parser& parser) {
        StructureNode* node = new StructureNode();

        assert(parser.next().type() == Token::Type::KeywordStructure);

        node->setVisibility(parser.context()->visibility());

        node->_packing = 0;
        if (parser.nextIf(":")) {
            // TODO: this needs to be more strict about the input types
            assert(parser.peek().type() == Token::Type::NumericLiteral);

            node->_packing = strtol(parser.next().str().c_str(), NULL, 10);
        }

        assert(parser.peek().type() == Token::Type::Identifier);

        // Here's an interesting problem.  If the struct refers to itself in the 
        // definition, it has to be already defined.  Additionally, in C, that's
        // not possible without referencing a specially-named version.
        DataType* type = new DataType(DataType::Flavor::Structure, parser.next().str());

        node->_type = type;

        parser.parseNewline();

        type->setNamespace(parser.currentScope()->fullNamespace());

        type->setCSourcePrependStructKeyword(true);

        for (Annotation* annotation : parser.currentScope()->annotations()) {
            annotation->applyToDataType(type);
        }
        parser.currentScope()->clearAnnotations();

        parser.currentModule()->addDataType(type);

        // parse the members, but be sure not to create scoped variables for them
        parser.parseUntilEnd([&] () {
            VariableDeclarationNode* var = VariableDeclarationNode::parse(parser, false);
            parser.parseNewline();

            node->addChild(var);

            // make sure to add a child to the structure type
            type->addChild(var->variable()->type(), var->variable()->name());
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
        return _type->fullyQualifiedName();
    }

    void StructureNode::codeGen(CSourceContext& context) {
        if (this->visibility() == TranslationUnit::Visibility::External) {
            // create an opaque structure declaration
            context.externalDeclarations()->print("typedef struct ");
            context.externalDeclarations()->print(this->structureName());
            context.externalDeclarations()->print(" ");
            context.externalDeclarations()->print(this->structureName());
            context.externalDeclarations()->printLine(";");
            context.externalDeclarations()->printLine("");
        }

        context.adjustCurrentForVisibility(TranslationUnit::Visibility::Internal, [&] (CSource* source) {
            if (this->packing() != 0) {
                std::stringstream s;

                // TODO: this is kinda stupid just to convert into a string...
                s << this->packing();

                source->printLine("#pragma pack(push)");
                source->printLine("#pragma pack(" + s.str() + ")");
            }

            source->print("typedef struct ");
            source->print(this->structureName());
            source->printLineAndIndent(" {");

            this->codeGenChildren(context);

            source->outdentAndPrintLine("} " + this->structureName() + ";");

            if (this->packing() != 0) {
                source->printLine("#pragma pack(pop)");
            }

            source->printLine(""); // one extra line of padding
        });
    }
}
