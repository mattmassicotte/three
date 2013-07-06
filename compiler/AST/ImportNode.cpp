#include "ImportNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    ImportNode* ImportNode::parse(Parser& parser) {
        ImportNode* node = new ImportNode();

        assert(parser.peek().type() == Token::Type::KeywordImport);
        parser.next();

        std::stringstream s;

        // TODO: this is infinite if end of input is reached
        while (parser.peek().type() != Token::Type::Newline) {
            s << parser.next().str();
        }

        node->setPath(s.str());
        node->_module = parser.moduleWithIdentifier(s.str());

        parser.parseNewline(true);

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

    Module* ImportNode::module() const {
        return this->_module;
    }

    void ImportNode::codeGenCSource(CSourceContext& context) {
        if (_module) {
            context.addHeader(_module->cIncludePath());
        }
    }
}
