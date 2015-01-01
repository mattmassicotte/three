#include "ASTVisitor.h"
#include "../AST.h"

namespace Three {
    ASTVisitor::ASTVisitor() {
    }

    void ASTVisitor::visitChildren(ASTNode& node) {
        node.eachChild([&] (ASTNode* child, uint32_t _) {
            child->accept(*this);
        });
    }

    void ASTVisitor::visit(class ASTNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class ExternalSourceNode& node) {
        std::cout << "Visiting external source" << std::endl;
    }

    void ASTVisitor::visit(class FunctionDefinitionNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class VariableDeclarationNode& node) {
        if (node.initializerExpression()) {
            node.initializerExpression()->accept(*this);
        }
    }

    void ASTVisitor::visit(class ClosureNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class IntegerLiteralNode& node) {
    }

    void ASTVisitor::visit(class BinaryOperatorNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class VariableNode& node) {
        std::cout << "Visiting variable" << std::endl;
    }

    void ASTVisitor::visit(class CapturedVariableNode& node) {
        std::cout << "Visiting captured" << std::endl;
    }

    void ASTVisitor::visit(class ReferencedVariableNode& node) {
        std::cout << "Visiting referenced variable " << std::endl;
    }

    void ASTVisitor::visit(class FunctionCallOperatorNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class MethodCallOperatorNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class UnaryOperatorNode& node) {
        std::cout << "Visiting unary op" << std::endl;
    }

    void ASTVisitor::visit(class StringLiteralNode& node) {
        std::cout << "Visiting string literal" << std::endl;
    }

    void ASTVisitor::visit(class SwitchNode& node) {
        std::cout << "Visiting switch" << std::endl;
    }

    void ASTVisitor::visit(IfNode& node) {
        node.condition()->accept(*this);
        this->visitChildren(node);
        if (node.elseStatement()) {
            this->visitChildren(*node.elseStatement());
        }
    }

    void ASTVisitor::visit(class StructureNode& node) {
        std::cout << "Visiting structure" << std::endl;
    }

    void ASTVisitor::visit(class EnumerationNode& node) {
        std::cout << "Visiting enumeration" << std::endl;
    }

    void ASTVisitor::visit(class MemberAccessNode& node) {
        std::cout << "Visiting member access" << std::endl;
    }

    void ASTVisitor::visit(class TernaryOperatorNode& node) {
        std::cout << "Visiting ternary operator" << std::endl;
    }

    void ASTVisitor::visit(class DestructuredAssignmentOperatorNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class IndexerNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class ReturnNode& node) {
        std::cout << "Visiting return" << std::endl;
    }

    void ASTVisitor::visit(class NullLiteralNode& node) {
        std::cout << "Visiting null literal" << std::endl;
    }

    void ASTVisitor::visit(class BooleanLiteralNode& node) {
        std::cout << "Visiting boolean literal" << std::endl;
    }

    void ASTVisitor::visit(class TupleNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class LoopNode& node) {
        std::cout << "Visiting loop" << std::endl;
    }

    void ASTVisitor::visit(class ForNode& node) {
        std::cout << "Visiting for" << std::endl;
    }

    void ASTVisitor::visit(class BreakNode& node) {
        std::cout << "Visiting break" << std::endl;
    }

    void ASTVisitor::visit(class ContinueNode& node) {
        std::cout << "Visiting continue" << std::endl;
    }

    void ASTVisitor::visit(class SizeofNode& node) {
        std::cout << "Visiting sizeof" << std::endl;
    }

    void ASTVisitor::visit(class AlignofNode& node) {
        std::cout << "Visiting alignof" << std::endl;
    }

    void ASTVisitor::visit(class OffsetofNode& node) {
        std::cout << "Visiting offsetof" << std::endl;
    }

    void ASTVisitor::visit(class ContainerofNode& node) {
        std::cout << "Visiting containerof" << std::endl;
    }

    void ASTVisitor::visit(class TypeofNode& node) {
        std::cout << "Visiting typeof" << std::endl;
    }

    void ASTVisitor::visit(class CardinalityofNode& node) {
        std::cout << "Visiting cardinalityof" << std::endl;
    }

    void ASTVisitor::visit(class CastNode& node) {
        std::cout << "Visiting cast" << std::endl;
    }

    void ASTVisitor::visit(class NextargNode& node) {
        this->visitChildren(node);
    }

    void ASTVisitor::visit(class AbortStatementNode& node) {
        std::cout << "Visiting abort" << std::endl;
    }
    void ASTVisitor::visit(class AtomicExpressionNode& node) {
        std::cout << "Visiting atomic expression" << std::endl;
    }

    void ASTVisitor::visit(class AtomicStatementNode& node) {
        std::cout << "Visiting atomic statement" << std::endl;
    }

    void ASTVisitor::visit(class BarrierNode& node) {
        std::cout << "Visiting barrier" << std::endl;
    }

    void ASTVisitor::visit(class ValueNode& node) {
        std::cout << "Visiting value" << std::endl;
    }

    void ASTVisitor::visit(class CMacroNode& node) {
        std::cout << "Visiting C macro" << std::endl;
    }
}
