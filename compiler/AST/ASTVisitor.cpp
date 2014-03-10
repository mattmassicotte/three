#include "ASTVisitor.h"

#include <iostream>

namespace Three {
    ASTVisitor::ASTVisitor() {
    }

    void ASTVisitor::visit(class ASTNode& node) {
        std::cout << "Visiting node" << std::endl;
    }

    void ASTVisitor::visit(class ExternalSourceNode& node) {
        std::cout << "Visiting external source" << std::endl;
    }

    void ASTVisitor::visit(class FunctionDefinitionNode& node) {
        std::cout << "Visiting function definition" << std::endl;
    }

    void ASTVisitor::visit(class VariableDeclarationNode& node) {
        std::cout << "Visiting variable declaration" << std::endl;
    }

    void ASTVisitor::visit(class ClosureNode& node) {
        std::cout << "Visiting closure" << std::endl;
    }

    void ASTVisitor::visit(class IntegerLiteralNode& node) {
        std::cout << "Visiting integer literal" << std::endl;
    }

    void ASTVisitor::visit(class BinaryOperatorNode& node) {
        std::cout << "Visiting binary operator" << std::endl;
    }

    void ASTVisitor::visit(class VariableNode& node) {
        std::cout << "Visiting variable" << std::endl;
    }

    void ASTVisitor::visit(class FunctionCallOperatorNode& node) {
        std::cout << "Visiting function call op" << std::endl;
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

    void ASTVisitor::visit(class IfNode& node) {
        std::cout << "Visiting if" << std::endl;
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

    void ASTVisitor::visit(class ReturnNode& node) {
        std::cout << "Visiting return" << std::endl;
    }

    void ASTVisitor::visit(class NullLiteralNode& node) {
        std::cout << "Visiting null literal" << std::endl;
    }

    void ASTVisitor::visit(class BooleanLiteralNode& node) {
        std::cout << "Visiting boolean literal" << std::endl;
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

    void ASTVisitor::visit(class CastNode& node) {
        std::cout << "Visiting cast" << std::endl;
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
}
