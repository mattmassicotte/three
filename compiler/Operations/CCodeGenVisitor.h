#pragma once

#include "compiler/AST/ASTVisitor.h"
#include "compiler/CodeGen/CSource.h"
#include "compiler/AST/Atomics/AtomicNode.h"

#include <functional>
#include <string>
#include <map>

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
        void visit(class ExternalSourceNode& node);
        void visit(class FunctionDefinitionNode& node);
        void visit(class VariableDeclarationNode& node);
        void visit(class ClosureNode& node);
        void visit(class IntegerLiteralNode& node);
        void visit(class BinaryOperatorNode& node);
        void visit(class VariableNode& node);
        void visit(class CapturedVariableNode& node);
        void visit(class ReferencedVariableNode& node);
        void visit(class FunctionCallOperatorNode& node);
        void visit(class MethodCallOperatorNode& node);
        void visit(class UnaryOperatorNode& node);
        void visit(class StringLiteralNode& node);
        void visit(class SwitchNode& node);
        void visit(class IfNode& node);
        void visit(class StructureNode& node);
        void visit(class EnumerationNode& node);
        void visit(class MemberAccessNode& node);
        void visit(class TernaryOperatorNode& node);
        void visit(class DestructuredAssignmentOperatorNode& node);
        void visit(class IndexerNode& node);
        void visit(class ReturnNode& node);
        void visit(class NullLiteralNode& node);
        void visit(class BooleanLiteralNode& node);
        void visit(class TupleNode& node);
        void visit(class LoopNode& node);
        void visit(class ForNode& node);
        void visit(class BreakNode& node);
        void visit(class ContinueNode& node);
        void visit(class SizeofNode& node);
        void visit(class AlignofNode& node);
        void visit(class OffsetofNode& node);
        void visit(class TypeofNode& node);
        void visit(class CardinalityofNode& node);
        void visit(class CastNode& node);
        void visit(class NextargNode& node);
        void visit(class AbortStatementNode& node);
        void visit(class AtomicExpressionNode& node);
        void visit(class AtomicStatementNode& node);
        void visit(class BarrierNode& node);
        void visit(class ValueNode& node);
        void visit(class CMacroNode& node);

        void visitChildren(class ASTNode& node, bool statements = false);

        std::string externalHeaderString();
        std::string internalHeaderString();
        std::string declarationsString();
        std::string bodyString();

        void addInternalHeader(bool relative, const std::string& path);
        void addDeclarationsHeader(bool relative, const std::string& path);

    private:
        void forCondition(class ForNode& node);
        void forStartExpression(class ForNode& node);
        void forLoopExpression(class ForNode& node);
        void returnStatementArgument(class ReturnNode& node);
        void prepareForAtomicExpressions();
        void prepareForTransactions();
        void transactionAllocation(const std::string& name, ASTNode* lockFunction, ASTNode* unlockFunction, ASTNode* context);
        void endCurrentTransaction(const std::string& name);
        std::string c11MemoryOrderString(AtomicNode::Ordering order) const;
        void atomicVariable(class OperatorNode* op);
        std::string c11AtomicFunctionForFullOperation(const std::string& op);
        bool c11AtomicFunctionIsLoadOperation(const std::string& op);

        void closureEnvironmentStructure(class ClosureNode& node, CSource& source);
        std::string closureEnvironmentCapture(class ClosureNode& node);

        void createDefinitionForTuple(const class DataType& tupleType, CSource& source);

        void sourceForVisibility(TranslationUnit::Visibility v, std::function<void (CSource&)> func);

    private:
        CSource _externalHeaderSource;
        CSource _internalHeaderSource;
        CSource _declaractionsSource;
        CSource _bodySource;

        CSource* _currentSource;

        uint32_t _tmpReturnValueCounter;
        DataType _currentFunctionType;
        std::map<std::string, bool> _tupleDefinitionMap;
    };
}
