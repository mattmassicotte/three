#include "FunctionDefinitionNode.h"
#include "../Parser.h"

namespace Language {
    FunctionDefinitionNode* FunctionDefinitionNode::parse(Parser& parser) {
        parser.next();

        return new FunctionDefinitionNode();
    }

    std::string FunctionDefinitionNode::name() const {
        return "FunctionDefinition";
    }
}
