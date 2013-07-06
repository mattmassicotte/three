#include "VariableDeclarationNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    VariableDeclarationNode* VariableDeclarationNode::parse(Parser& parser, bool createVariable) {
        VariableDeclarationNode* node = new VariableDeclarationNode();

        node->_variable = new Variable();

        node->_variable->setType(parser.parseType());

        assert(parser.peek().type() == Token::Type::Identifier);
        node->_variable->setName(parser.next().str());

        if (createVariable) {
            parser.currentScope()->addVariable(node->_variable->name(), node->_variable);
        }

        return node;
    }

    std::string VariableDeclarationNode::name() const {
        return "VariableDeclaration";
    }

    Variable* VariableDeclarationNode::variable() const {
        return this->_variable;
    }

    void VariableDeclarationNode::codeGenCSource(CSourceContext& context) {
        this->_variable->type().codeGenCSource(context.current(), this->_variable->name());
        context << ";";
    }
}