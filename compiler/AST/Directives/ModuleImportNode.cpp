#include "ModuleImportNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    ModuleImportNode* ModuleImportNode::parse(Parser& parser) {
        ModuleImportNode* node = new ModuleImportNode();

        assert(parser.next().type() == Token::Type::KeywordImport);

        node->setPath(parser.parseQualifiedIdentifier());
        assert(node->_path.length() > 0);

        parser.parseNewline();

        parser.currentModule()->importModule(node->_path);

        return node;
    }

    std::string ModuleImportNode::name() const {
        return "ModuleImport";
    }

    void ModuleImportNode::setPath(const std::string& value) {
        _path = value;
    }

    std::string ModuleImportNode::path() const {
        return _path;
    }
}
