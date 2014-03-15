#include "MissingReturnChecker.h"
#include "compiler/AST.h"

namespace Three {
    MissingReturnChecker::MissingReturnChecker() {
        _returnFound = false;
    }

    void MissingReturnChecker::visit(FunctionDefinitionNode& node) {
        _returnFound = false;

        // scan the last node, if there is one
        if (node.lastChild()) {
            node.lastChild()->accept(*this);
        }

        if (!_returnFound) {
            std::cout << "Return node missing" << std::endl;
        }
    }

    void MissingReturnChecker::visit(ClosureNode& node) {
        // TODO: this needs to happen too
    }

    void MissingReturnChecker::visit(ReturnNode& node) {
        _returnFound = true;
    }
}
