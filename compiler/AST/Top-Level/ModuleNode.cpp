#include "ModuleNode.h"
#include "IncludeNode.h"
#include "LinkageNode.h"
#include "VisibilityNode.h"
#include "DefinitionNode.h"
#include "../Directives/ImportNode.h"
#include "../Types/StructureNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ModuleNode* ModuleNode::parse(Parser& parser) {
        ModuleNode* node = new ModuleNode();

        assert(parser.next().type() == Language::Token::Type::KeywordModule);

        node->_name = parser.parseQualifiedName();
        assert(node->_name.length() > 0);

        node->_module = new Module();

        parser.context()->pushModule(node->_module);

        parser.parseNewline();

        parser.parseUntilEnd([&] () {
            node->addChild(ModuleNode::parseSubmoduleStatement(parser));
        });

        parser.context()->popModule();

        return node;
    }

    ASTNode* ModuleNode::parseSubmoduleStatement(Parser& parser) {
        switch (parser.peek().type()) {
            case Token::Type::KeywordDef:
                return DefinitionNode::parse(parser, true);
            case Token::Type::KeywordImport:
                return ImportNode::parse(parser);
            case Token::Type::Identifier:
                if (parser.peek().str() == "public") {
                    return VisibilityNode::parse(parser);
                } else if (parser.peek().str() == "include") {
                    return IncludeNode::parse(parser);
                }
                break;
            case Token::Type::KeywordStructure:
                return StructureNode::parse(parser);
            case Token::Type::KeywordLinkage:
                return LinkageNode::parse(parser);
            default:
                break;
        }

        std::cout << "Submodule parsing: unrecognized token '" << parser.peek().str() << "'" << std::endl;

        return NULL;
    }

    std::string ModuleNode::name() const {
        return "Module";
    }

    std::string ModuleNode::moduleName() const {
        return _name;
    }

    void ModuleNode::codeGenCSource(CSourceContext& context) {
    }
}
