#include "VariableNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    VariableNode* VariableNode::parse(Parser& parser) {
        VariableNode* node = new VariableNode();

        assert(parser.peek().type() == Token::Type::Identifier);

        node->_variable = parser.currentScope()->variableForName(parser.next().str());

        return node;
    }

    std::string VariableNode::name() const {
        return "Variable";
    }

    void VariableNode::codeGenCSource(CSourceContext& context) {
        context.print(this->_variable->name());
    }
}
