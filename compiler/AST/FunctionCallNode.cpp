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

}
