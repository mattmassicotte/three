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

        assert(parser.next().type() == Token::Type::Newline);
        while (parser.peek().type() == Token::Type::Newline) {
            parser.next();
        }

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

    void ImportNode::codeGenCSource(CSourceContext& context) {
        if (_module) {
            context.print("#include <");
            context.print(_module->cIncludePath());
            context.print(">");
        }
    }
}
