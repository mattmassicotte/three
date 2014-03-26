#pragma once

namespace Three {
    class ASTVisitor {
    public:
        ASTVisitor();

        virtual void visitChildren(class ASTNode& node);

        virtual void visit(class ASTNode& node);
        virtual void visit(class ExternalSourceNode& node);
        virtual void visit(class FunctionDefinitionNode& node);
        virtual void visit(class VariableDeclarationNode& node);
        virtual void visit(class ClosureNode& node);
        virtual void visit(class IntegerLiteralNode& node);
        virtual void visit(class BinaryOperatorNode& node);
        virtual void visit(class VariableNode& node);
        virtual void visit(class FunctionCallOperatorNode& node);
        virtual void visit(class UnaryOperatorNode& node);
        virtual void visit(class StringLiteralNode& node);
        virtual void visit(class SwitchNode& node);
        virtual void visit(class IfNode& node);
        virtual void visit(class StructureNode& node);
        virtual void visit(class EnumerationNode& node);
        virtual void visit(class MemberAccessNode& node);
        virtual void visit(class TernaryOperatorNode& node);
        virtual void visit(class IndexerNode& node);
        virtual void visit(class ReturnNode& node);
        virtual void visit(class NullLiteralNode& node);
        virtual void visit(class BooleanLiteralNode& node);
        virtual void visit(class LoopNode& node);
        virtual void visit(class ForNode& node);
        virtual void visit(class BreakNode& node);
        virtual void visit(class ContinueNode& node);
        virtual void visit(class SizeofNode& node);
        virtual void visit(class CastNode& node);
        virtual void visit(class NextargNode& node);
        virtual void visit(class AbortStatementNode& node);
        virtual void visit(class AtomicExpressionNode& node);
        virtual void visit(class AtomicStatementNode& node);
        virtual void visit(class BarrierNode& node);
        virtual void visit(class ValueNode& node);
    };
}
