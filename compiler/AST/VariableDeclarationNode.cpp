#include "VariableDeclarationNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    VariableDeclarationNode* VariableDeclarationNode::parse(Parser& parser) {
        VariableDeclarationNode* node = new VariableDeclarationNode();

        node->_variable = new Variable();

        node->_variable->setType(parser.parseType());

        assert(parser.peek().type() == Token::Type::Identifier);
        node->_variable->setName(parser.next().str());

        parser.currentScope()->addVariable(node->_variable->name(), node->_variable);

        return node;
    }

    std::string VariableDeclarationNode::name() const {
        return "VariableDeclaration";
    }

    void VariableDeclarationNode::codeGenCSource(CSourceContext& context) {
        this->_variable->type().codeGenCSource(context.current(), this->_variable->name());
        context << ";";
    }
}
