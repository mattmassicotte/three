#include "VariableNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    VariableNode* VariableNode::parse(Parser& parser, const std::string& identifier) {
        VariableNode* node = new VariableNode();

        // TODO: a VariableReference type could help simply this significantly
        // TODO: this is not really a parse...
        node->_variable   = parser.currentScope()->variableForName(identifier);
        node->_referenced = parser.currentScope()->referencedVariable(identifier);
        node->_closed     = parser.currentScope()->closedVariable(identifier);

        return node;
    }

    VariableNode::VariableNode() : _variable(NULL), _referenced(false), _closed(false) {
    }

    TypeReference VariableNode::nodeType() const {
        return this->variable()->type();
    }

    std::string VariableNode::name() const {
        return "Variable";
    }

    Variable* VariableNode::variable() const {
        return _variable;
    }

    void VariableNode::setVariable(Variable* var) {
        assert(var);
        _variable = var;
    }

    void VariableNode::codeGen(CSourceContext& context) {
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
