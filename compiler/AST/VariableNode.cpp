#include "VariableNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    VariableNode* VariableNode::parse(Parser& parser) {
        VariableNode* node = new VariableNode();

        node->_variable = new Variable();

        node->_variable->setType(parser.parseType());

        std::cout << parser.peek().str() << std::endl;
        assert(parser.peek().type() == Token::Type::Identifier);
        node->_variable->setName(parser.next().str());

        parser.currentScope()->addVariable(node->_variable->name(), node->_variable);

        return node;
    }

    std::string VariableNode::name() const {
        return "Variable";
    }

    void VariableNode::codeGenCSource(CSourceContext& context) {
        DataType* type = this->_variable->type();

        if (!type->isFunction()) {
            context.print(type->name());
            context.print(" ");
            context.print(this->_variable->name());
            context.print(";");
            return;
        }

        // return (*varName)(param1, param2);
        context.print(type->returnType()->name());
        context.print("(*");
        context.print(this->_variable->name()); // variable name
        context.print(")(");

        // parameters
        type->eachParameterWithLast([=, &context] (DataType* param, bool last) {
            context.print(param->name());
            if (!last) {
                context.print(", ");
            }
        });

        context.print(");");
    }
}
