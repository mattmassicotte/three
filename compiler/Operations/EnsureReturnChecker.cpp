#include "EnsureReturnChecker.h"
#include "compiler/AST.h"

namespace Three {
    EnsureReturnChecker::EnsureReturnChecker() {
        _returnsAllowed = true;
    }

    void EnsureReturnChecker::visit(FunctionDefinitionNode& node) {
        // if (node.ensureClause()) {
        //     _returnsAllowed = false;
        //     node.ensureClause()->accept(*this);
        // }

        _returnsAllowed = true;
    }

    void EnsureReturnChecker::visit(ClosureNode& node) {
        bool tmp = _returnsAllowed;

        _returnsAllowed = true;

        ASTVisitor::visit(node); // call super

        _returnsAllowed = tmp;
    }

    void EnsureReturnChecker::visit(ReturnNode& node) {
        if (_returnsAllowed) {
            return;
        }

        std::cout << "Found an illegal return" << std::endl;
    }
}
