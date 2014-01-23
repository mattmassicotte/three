#pragma once

#include "compiler/AST/ASTVisitor.h"
#include "compiler/CodeGen/CSource.h"
#include "compiler/AST/Atomics/AtomicNode.h"

#include <functional>
#include <string>

namespace Three {
    class CCodeGenVisitor : public ASTVisitor {
    public:
        CCodeGenVisitor();

    private:
        CCodeGenVisitor(const CCodeGenVisitor& other);
        CCodeGenVisitor(CCodeGenVisitor&& other);
        CCodeGenVisitor& operator=(CCodeGenVisitor other);
        CCodeGenVisitor& operator=(CCodeGenVisitor&& other);

    public:
        void visit(class ASTNode& node);
        void visit(class IncludeNode& node);
        void visit(class FunctionDefinitionNode& node);
        void visit(class VariableDeclarationNode& node);
        void visit(class ClosureNode& node);
        void visit(class IntegerLiteralNode& node);
        void visit(class BinaryOperatorNode& node);
        void visit(class VariableNode& node);
        void visit(class FunctionCallOperatorNode& node);
        void visit(class UnaryOperatorNode& node);
        void visit(class StringLiteralNode& node);
        void visit(class SwitchNode& node);
        void visit(class IfNode& node);
        void visit(class StructureNode& node);
        void visit(class EnumerationNode& node);
        void visit(class MemberAccessNode& node);
        void visit(class TernaryOperatorNode& node);
        void visit(class ReturnNode& node);
        void visit(class NullLiteralNode& node);
        void visit(class BooleanLiteralNode& node);
        void visit(class LoopNode& node);
        void visit(class ForNode& node);
        void visit(class BreakNode& node);
        void visit(class ContinueNode& node);
        void visit(class SizeofNode& node);
        void visit(class AbortStatementNode& node);
        void visit(class AtomicExpressionNode& node);
        void visit(class AtomicStatementNode& node);
        void visit(class BarrierNode& node);

        void visitChildren(class ASTNode& node);

        std::string externalHeaderString();
        std::string internalHeaderString();
        std::string declarationsString();
        std::string bodyString();

    private:
        void forCondition(class ForNode& node);
        void forStartExpression(class ForNode& node);
        void forLoopExpression(class ForNode& node);
        void prepareForAtomicExpressions();
        void prepareForTransactions();
        void transactionAllocation(const std::string& name);
        std::string c11MemoryOrderString(AtomicNode::Ordering order) const;
        void atomicVariable(class OperatorNode* op);
        std::string c11AtomicFunctionForFullOperation(const std::string& op);
        bool c11AtomicFunctionIsLoadOperation(const std::string& op);

        void sourceForVisibility(TranslationUnit::Visibility v, std::function<void (CSource&)> func);

    private:
        CSource _externalHeaderSource;
        CSource _internalHeaderSource;
        CSource _declaractionsSource;
        CSource _bodySource;

        CSource* _currentSource;
    };
}
