#include "ImportNode.h"
#include "../../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    ImportNode* ImportNode::parse(Parser& parser) {
        ImportNode* node = new ImportNode();

        assert(parser.next().type() == Token::Type::KeywordImport);

        node->setPath(parser.parseQualifiedName());
        assert(node->_path.length() > 0);

        // TODO: not exactly right.  This might not be the current translation unit
        node->_module = parser.currentModule()->importModule(node->path(), std::vector<std::string>());

        parser.parseNewline();

        return node;
    }

    std::string ImportNode::name() const {
        return "Import";
    }

    std::string ImportNode::str() const {
        std::stringstream s;

        s << this->name() << ": \"" << this->path() << "\"";

        return s.str();
    }

    void ImportNode::setPath(const std::string& value) {
        _path = value;
    }

    std::string ImportNode::path() const {
        return _path;
    }

    Three::Module* ImportNode::module() const {
        return this->_module;
    }

    void ImportNode::codeGenCSource(CSourceContext& context) {
        _module->eachCIncludePath([&] (const std::string& path) {
            std::cout << "Header: " << path << std::endl;
            context.addHeader(path);
        });
    }
}
