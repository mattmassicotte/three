#pragma once

#include "compiler/AST/ASTVisitor.h"

#include <string>

namespace Three {
    class MissingReturnChecker : public ASTVisitor {
    public:
        MissingReturnChecker();

    private:
        MissingReturnChecker(const MissingReturnChecker& other);
        MissingReturnChecker(MissingReturnChecker&& other);
        MissingReturnChecker& operator=(MissingReturnChecker other);
        MissingReturnChecker& operator=(MissingReturnChecker&& other);

    public:
        void visit(class FunctionDefinitionNode& node);
        void visit(class ClosureNode& node);
        void visit(class ReturnNode& node);

    private:
        bool _returnFound;
    };
}
