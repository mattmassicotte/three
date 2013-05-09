#include "FunctionDefinitionNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
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

            node->_parameterTypes.push_back(DataType::parse(parser));

            // the parameter identifier
            parser.next().str();

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

            node->_returnType = DataType::parse(parser);
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

    std::vector<DataType> FunctionDefinitionNode::parameters() const {
        return _parameterTypes;
    }

    DataType FunctionDefinitionNode::returnType() const {
        return _returnType;
    }

    std::string FunctionDefinitionNode::str() const {
        std::stringstream s;

        s << this->name() << ": " << this->functionName();

        s << " params: ";
        for (DataType dt : _parameterTypes) {
            s << "'" << dt.str() << "', ";
        }

        s << "return: " << _returnType.str();

        return s.str();
    }

}
