#include "VariableDeclarationNode.h"
#include "../Parser.h"

#include <assert.h>

namespace Language {
    VariableDeclarationNode* VariableDeclarationNode::parse(Parser& parser) {
        VariableDeclarationNode* node = new VariableDeclarationNode();

        node->_variable = new Variable();

        node->_variable->setType(parser.parseType());

        std::cout << parser.peek().str() << std::endl;
        assert(parser.peek().type() == Token::Type::Identifier);
        node->_variable->setName(parser.next().str());

        parser.currentScope()->addVariable(node->_variable->name(), node->_variable);

        return node;
    }

    std::string VariableDeclarationNode::name() const {
        return "VariableDeclaration";
    }

    void VariableDeclarationNode::codeGenCSource(CSourceContext& context) {
        DataType* type = this->_variable->type().referencedType();

        if (!type->isFunction()) {
            context.print(type->name());
            context.print(" ");
            context.print(this->_variable->name());
            context.print(";");
            return;
        }

        // return (*varName)(param1, param2);
        context.print(type->returnType().referencedType()->name());
        context.print("(*");
        context.print(this->_variable->name()); // variable name
        context.print(")(");

        // parameters
        type->eachParameterWithLast([=, &context] (const TypeReference& param, bool last) {
            context.print(param.referencedType()->name());
            if (!last) {
                context.print(", ");
            }
        });

        context.print(");");
    }
}
