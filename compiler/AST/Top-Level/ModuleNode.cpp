#include "ModuleNode.h"
#include "../Directives/IncludeNode.h"
#include "../Directives/LinkageNode.h"
#include "compiler/AST/Directives/VisibilityNode.h"
#include "DefinitionNode.h"
#include "../Directives/ModuleImportNode.h"
#include "StructureNode.h"

#include <assert.h>

namespace Three {
    // ModuleNode* ModuleNode::parse(OldParser& parser) {
    //     ModuleNode* node = new ModuleNode();
    // 
    //     assert(parser.next().type() == Token::Type::KeywordModule);
    // 
    //     node->_name = parser.parseQualifiedIdentifier();
    //     assert(node->_name.length() > 0);
    //     parser.currentModule()->name = node->_name;
    // 
    //     parser.pushScope(new Scope(node->_name));
    //     parser.currentScope()->setNamespace(node->_name);
    // 
    //     parser.parseNewline();
    // 
    //     parser.parseUntilEnd([&] () {
    //         node->addChild(ModuleNode::parseSubmoduleStatement(parser));
    //     });
    // 
    //     parser.popScope();
    // 
    //     parser.parseNewline();
    // 
    //     return node;
    // }

    // ASTNode* ModuleNode::parseSubmoduleStatement(OldParser& parser) {
    //     switch (parser.peek().type()) {
    //         case Token::Type::KeywordDef:
    //             return DefinitionNode::parse(parser, true);
    //         case Token::Type::KeywordImport:
    //             return ModuleImportNode::parse(parser);
    //         case Token::Type::Identifier:
    //             if (parser.peek().str() == "public") {
    //                 return VisibilityNode::parse(parser);
    //             } else if (parser.peek().str() == "include") {
    //                 return IncludeNode::parse(parser);
    //             }
    //             break;
    //         case Token::Type::KeywordStructure:
    //             return StructureNode::parse(parser);
    //         case Token::Type::KeywordLinkage:
    //             return LinkageNode::parse(parser);
    //         default:
    //             break;
    //     }
    // 
    //     std::cout << "Submodule parsing: unrecognized token '" << parser.peek().str() << "'" << std::endl;
    // 
    //     return NULL;
    // }

    std::string ModuleNode::name() const {
        return "Module";
    }

    std::string ModuleNode::moduleName() const {
        return _name;
    }
}
