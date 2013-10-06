#include "FunctionCallNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Three {
    FunctionCallNode* FunctionCallNode::parse(Parser& parser, const std::string& functionName, ASTNode* firstArgument) {
        FunctionCallNode* node = new FunctionCallNode();

        assert(functionName.length() > 0);

        node->_functionType = NULL;
        Function* func = parser.currentModule()->functionForName(functionName);
        if (func) {
            node->setFunctionName(func->fullyQualifiedName());
        } else {
            Variable* v = parser.currentScope()->variableForName(functionName);
            
            if (!v) {
                std::cout << "Unable to find match for called function: " << functionName << std::endl;
            }

            assert(v && "Must find a function or variable match for a function call");
            node->_functionType = v->type().referencedType();

            node->setFunctionName(functionName);
        }

        assert(parser.next().type() == Token::Type::PunctuationOpenParen);

        if (firstArgument) {
            node->addChild(firstArgument);
        }

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

    FunctionCallNode* FunctionCallNode::parseMethod(Parser& parser, ASTNode* target) {
        assert(parser.peek().type() == Token::Type::Identifier);

        std::string functionName = target->nodeType().name() + "_3_" + parser.next().str();

        return FunctionCallNode::parse(parser, functionName, target);
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

    TypeReference FunctionCallNode::functionType() const {
        return TypeReference(_functionType, 1);
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
        if (this->isClosure()) {
            context << "THREE_CALL_CLOSURE(";
            this->functionType().codeGenCSourceFunctionType(context.current(), "");
            context << ", ";
            context << this->functionName();

            if (this->childCount() > 0) {
                context << ", ";
            }
        } else {
            context << this->functionName() << "(";
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
