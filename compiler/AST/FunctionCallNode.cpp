#include "FunctionCallNode.h"
#include "../Parser.h"

#include <assert.h>
#include <sstream>

namespace Language {
    FunctionCallNode* FunctionCallNode::parse(Parser& parser) {
        FunctionCallNode* node;

        node = new FunctionCallNode();

        node->setFunctionName(parser.next().str());

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

    void FunctionCallNode::renderCCode(std::stringstream& stream, uint32_t indentation) {
        stream << std::string(indentation*4, ' ');

        stream << this->functionName() << "(";

        for (int i = 0; i < this->childCount() - 1; ++i) {
            this->childAtIndex(i)->renderCCode(stream, indentation+1);
            stream << ", ";
        }

        this->childAtIndex(this->childCount() - 1)->renderCCode(stream, indentation+1);

        stream << ")";

        if (this->statement()) {
            stream << ";" << std::endl;
        }
    }
}
