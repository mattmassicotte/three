#pragma once

#include "compiler/AST/ASTVisitor.h"

#include <string>

namespace Three {
    class EnsureReturnChecker : public ASTVisitor {
    public:
        EnsureReturnChecker();

    private:
        EnsureReturnChecker(const EnsureReturnChecker& other);
        EnsureReturnChecker(EnsureReturnChecker&& other);
        EnsureReturnChecker& operator=(EnsureReturnChecker other);
        EnsureReturnChecker& operator=(EnsureReturnChecker&& other);

    public:
        void visit(class FunctionDefinitionNode& node);
        void visit(class ClosureNode& node);
        void visit(class ReturnNode& node);

    private:
        bool _returnsAllowed;
    };
}
