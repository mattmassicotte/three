#include "FunctionDeclarationNode.h"
#include "../../Parser.h"

#include <assert.h>

namespace Three {
    FunctionDeclarationNode* FunctionDeclarationNode::parse(Parser& parser) {
        FunctionDeclarationNode* node = new FunctionDeclarationNode();

        // take (and then clear) current annotations
        node->setAnnotations(parser.currentScope()->annotations());
        parser.currentScope()->clearAnnotations();

        node->_function = parser.parseFunctionSignature();
        assert(node->_function);

        parser.parseNewline(true);

        return node;
    }

    std::string FunctionDeclarationNode::name() const {
        return "FunctionDeclaration";
    }

    Function* FunctionDeclarationNode::function() const {
        return _function;
    }

    void FunctionDeclarationNode::codeGenCSource(CSourceContext& context) {
    }
}
