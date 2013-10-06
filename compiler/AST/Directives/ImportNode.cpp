#include "ImportNode.h"
#include "../../Parser.h"
#include "../../CSourceIndexer.h"

#include <assert.h>
#include <sstream>

namespace Three {
    ImportNode* ImportNode::parse(Parser& parser) {
        ImportNode* node = new ImportNode();

        assert(parser.next().type() == Token::Type::KeywordImport);

        node->setPath(parser.parseQualifiedIdentifier());
        assert(node->_path.length() > 0);
        node->_visibility = parser.context()->visibility();

        parser.parseNewline();

        // TODO: This is copy-paste from the include node.
        CSourceIndexer index;

        if (!index.indexFileAtPath(node->resolvedFilePath())) {
            std::cout << "Unable to import '" << node->path() << "'" << std::endl;
            assert(0);
        }

        parser.currentModule()->addModule(node->resolvedFilePath(), index.module());

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

    std::string ImportNode::resolvedFilePath() const {
        std::string filePath = this->path() + ".h";

        return filePath;
    }

    Three::Module* ImportNode::module() const {
        return this->_module;
    }

    void ImportNode::codeGen(CSourceContext& context) {
        context.adjustCurrentForVisibility(this->_visibility, [&] (CSource* source) {
            source->addHeader(true, this->path() + ".h");
        });
    }
}
