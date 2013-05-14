#include "FunctionCallNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    FunctionCallNode* FunctionCallNode::parse(Parser& parser) {
        FunctionCallNode* node = new FunctionCallNode();
        Token t;

        t = parser.peek();
        assert(t.type() == Token::Type::Identifier);
        parser.next(); // advance past the identifier

        Function* func = parser.currentScope()->functionForName(t.str());
        if (func) {
            std::cout << "Found function: " << func->name() << std::endl;
        } else {
            std::cout << "Unable to find match for function: " << t.str() << std::endl;
        }

        node->setFunctionName(t.str());

        assert(parser.next().str().at(0) == '(');

        // now, we need to parse the arguments, which of which
        // is an expression

        while (parser.peek().str().at(0) != ')') {
            node->addChild(parser.parseExpression());

            if (parser.peek().str().at(0) == ',') {
                assert(parser.next().str().at(0) == ',');
            }
        }

        assert(parser.next().str().at(0) == ')');

        if (parser.peek().type() == Token::Type::Newline) {
            node->setStatement(true);
        }

        return node;
    }

    std::string FunctionCallNode::name() const {
        return "FunctionCall";
    }

    void FunctionCallNode::setFunctionName(const std::string& name) {
        _functionName = name;
    }

    std::string FunctionCallNode::functionName() const {
        return _functionName;
    }

    std::string FunctionCallNode::str() const {
        std::stringstream s;

        s << this->name() << ": " << this->functionName();

        return s.str();
    }

    void FunctionCallNode::codeGenCSource(CSourceContext& context) {
        context.print(this->functionName());
        context.print("(");

        this->eachChild([=, &context] (ASTNode* node, uint32_t index) {
            node->codeGenCSource(context);
            if (index < this->childCount() - 1) {
                context.print(", ");
            }
        });

        context.print(")");

        if (this->statement()) {
            context.print(";");
        }
    }
}
