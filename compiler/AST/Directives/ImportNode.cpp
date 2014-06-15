#include "ImportNode.h"
#include "../../CSourceIndexer.h"
#include "compiler/Parser/NewParser.h"

#include "compiler/Messages/ImportFailureMessage.h"

#include <assert.h>
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace Three {
    ImportNode* ImportNode::parse(NewParser& parser) {
        ImportNode* node = new ImportNode();

        if (!parser.helper()->nextIf(Token::Type::KeywordImport)) {
            assert(0 && "Message: Failed to parse import statement");
        }

        node->_argument = parser.parseMultiPartIdentifier();
        if (node->_argument.length() == 0) {
            assert(0 && "Message: Unable to parse import statement argument");
        }

        if (!parser.helper()->parseNewline()) {
            assert(0 && "Message: Import statement must be followed by a newline");
        }

        if (!parser.context()->import(node->_argument)) {
            parser.context()->addMessage(new ImportFailureMessage(node->_argument));
        }

        return node;
    }

    std::string ImportNode::nodeName() const {
        return "Import";
    }

    void ImportNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string ImportNode::name() const {
        return "Import";
    }

    std::string ImportNode::str() const {
        std::stringstream s;

        s << this->name() << ": \"" << this->path() << "\"";

        return s.str();
    }

    std::string ImportNode::headerName() const {
        return this->argument() + ".h";
    }

    bool ImportNode::relative() const {
        return true;
    }

    void ImportNode::setPath(const std::string& value) {
        _path = value;
    }

    std::string ImportNode::path() const {
        return _path;
    }

    std::string ImportNode::resolvedFilePath() const {
        std::stringstream s;

        // determine our current working directory
        size_t size = pathconf(".", _PC_PATH_MAX);
        char*  buf  = (char*)malloc(size);
        char*  path = getcwd(buf, size);

        s << path;

        free(buf);

        s << "/";
        s << this->path();
        s << ".h";

        return s.str();
    }
}
