#include "ImportNode.h"
#include "../../CSourceIndexer.h"
#include "compiler/Parser/NewParser.h"

#include <assert.h>
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace Three {
    // ImportNode* ImportNode::parse(OldParser& parser) {
    //     ImportNode* node = new ImportNode();
    // 
    //     assert(parser.next().type() == Token::Type::KeywordImport);
    // 
    //     node->setPath(parser.parseQualifiedIdentifier("/"));
    //     assert(node->_path.length() > 0);
    //     node->_visibility = parser.context()->visibility();
    // 
    //     parser.parseNewline();
    // 
    //     // TODO: This is copy-paste from the include node.
    //     CSourceIndexer index;
    // 
    //     if (!index.indexFileAtPath(node->resolvedFilePath())) {
    //         std::cout << "[Warning] Unable to import '" << node->path() << "'" << std::endl;
    //         return node;
    //     }
    // 
    //     parser.currentModule()->addModule(node->resolvedFilePath(), index.module());
    // 
    //     index.module()->eachFunction([&] (const Function* function) {
    //         Variable* var = new Variable();
    // 
    //         var->setName(function->fullyQualifiedName());
    //         var->setType(TypeReference(function->createType(), 1));
    // 
    //         parser.currentScope()->addVariable(function->fullyQualifiedName(), var);
    //     });
    // 
    //     return node;
    // }

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
