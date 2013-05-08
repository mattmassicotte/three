#include "FunctionDefinitionNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    std::string FunctionDefinitionNode::parseType(Parser& parser) {
        std::stringstream s;

        // a type, followed by an identifier

        // parse '*', if they are there
        while (parser.peek().str().at(0) == '*') {
            s << parser.next().str();
        }

        // parse the type identifier
        s << parser.next().str();

        // parser ":<specialization>", if present
        while (parser.peek().str().at(0) == ':') {
            s << parser.next().str();

            s << parser.next().str();
        }

        return s.str();
    }

    std::string FunctionDefinitionNode::parseParameter(Parser& parser) {
        std::stringstream s;

        s << FunctionDefinitionNode::parseType(parser);

        // finally, read the actual identifier
        s << " ";

        s << parser.next().str();

        return s.str();
    }
    
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        FunctionDefinitionNode* node;
        Token t;

        node = new FunctionDefinitionNode();

        // parse the function name
        t = parser.next();
        assert(t.type() == Token::Type::Identifier);

        node->setFunctionName(t.str());

        // move past the opening '('
        assert(parser.next().str().at(0) == '(');

        // at each point we could have:
        // - a ';'
        // - a Type + identifier, followed by a ';'
        // - a Type + identifier, followed by a ','
        // - a ')'
        while (true) {
            Token t = parser.peek();

            if (t.str().at(0) == ';' || t.str().at(0) == ')') {
                break;
            }

            node->_parameterTypes.push_back(parseParameter(parser));

            // a ',' means another paramter was specified
            if (parser.peek().str().at(0) == ',') {
                assert(parser.next().str().at(0) == ',');

                continue;
            }
        }

        // now, check for the return type
        if (parser.peek().str().at(0) == ';' && parser.peek(2).str().at(0) != ')') {
            // move past the ';'
            assert(parser.next().str().at(0) == ';');

            node->_returnType = parseType(parser);
        }

        // parse the close paren
        assert(parser.next().str().at(0) == ')');
        assert(parser.next().type() == Token::Type::Newline);

        // and now, parse the body
        while (true) {
            if (parser.peek().type() == Token::Type::KeywordEnd) {
                break;
            }

            node->addChild(parser.parseStatement());
        }

        assert(parser.next().type() == Token::Type::KeywordEnd);
        assert(parser.next().type() == Token::Type::Newline);

        return node;
    }

    std::string FunctionDefinitionNode::name() const {
        return "FunctionDefinition";
    }

    void FunctionDefinitionNode::setFunctionName(const std::string& name) {
        _functionName = name;
    }

    std::string FunctionDefinitionNode::functionName() const {
        return _functionName;
    }

    std::string FunctionDefinitionNode::str() const {
        std::stringstream s;

        s << this->name() << ": " << this->functionName();

        s << " params: ";
        for (std::string str : _parameterTypes) {
            s << "'" << str << "', ";
        }

        s << "return: " << _returnType;

        return s.str();
    }

}
