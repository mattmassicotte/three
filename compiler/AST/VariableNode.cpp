#include "VariableNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    VariableNode* VariableNode::parse(Parser& parser) {
        VariableNode* node = new VariableNode();

        assert(parser.peek().type() == Token::Type::Identifier);

        // TODO: a VariableReference type could help simply this significantly
        node->_variable   = parser.currentScope()->variableForName(parser.next().str());
        node->_referenced = parser.currentScope()->referencedVariable(node->_variable->name());
        node->_closed     = parser.currentScope()->closedVariable(node->_variable->name());

        return node;
    }

    std::string VariableNode::name() const {
        return "Variable";
    }

    void VariableNode::codeGenCSource(CSourceContext& context) {
        assert(this->_variable);

        if (this->_referenced) {
            assert(!this->_closed);
            context << "*(self_env->" << this->_variable->name() << ")";
            return;
        }

        if (this->_closed) {
            context << "(self_env->" << this->_variable->name() << ")";
            return;
        }

        context << this->_variable->name();
    }
}
