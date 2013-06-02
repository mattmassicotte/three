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

        node->_functionType = NULL;
        Function* func = parser.currentModule()->functionForName(t.str());
        if (!func) {
            Variable* v = parser.currentScope()->variableForName(t.str());
            
            if (!v) {
                std::cout << "Unable to find match for called function: " << t.str() << std::endl;
            } else {
                node->_functionType = v->type().referencedType();
            }
        }

        node->setFunctionName(t.str());

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        // now, we need to parse the arguments, which of which
        // is an expression

        while (parser.peek().str().at(0) != ')') {
            node->addChild(parser.parseExpression());

            if (parser.peek().str().at(0) == ',') {
                assert(parser.next().str().at(0) == ',');
            }
        }

        assert(parser.next().type() == Token::Type::PunctuationCloseParen);

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

    bool FunctionCallNode::isClosure() const {
        return _functionType && _functionType->flavor() == DataType::Flavor::Closure;
    }

    void FunctionCallNode::codeGenCSource(CSourceContext& context) {
        context << this->functionName();

        if (this->isClosure()) {
            context << ".function(";
            context << this->functionName() << ".env";
            if (this->childCount() > 0) {
                context << ", ";
            }
        } else {
            context << "(";
        }

        this->eachChild([=, &context] (ASTNode* node, uint32_t index) {
            node->codeGenCSource(context);
            if (index < this->childCount() - 1) {
                context << ", ";
            }
        });

        context << ")";

        if (this->statement()) {
            context << ";";
        }
    }
}
