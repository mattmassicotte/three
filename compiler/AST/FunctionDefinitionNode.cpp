#include "FunctionDefinitionNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        FunctionDefinitionNode* node;
        Token t;

        node = new FunctionDefinitionNode();

        t = parser.next();
        assert(t.type() == Token::Type::Identifier);

        node->setFunctionName(t.str());

        assert(parser.next().str().at(0) == '(');

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

        return s.str();
    }

}
