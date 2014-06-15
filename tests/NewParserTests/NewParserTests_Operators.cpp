#include "../ParserTestsBase.h"
#include "compiler/constructs/NewDataType.h"

class NewParserTests_Operators : public ParserTestsBase {
};

TEST_F(NewParserTests_Operators, MultiplyExpression) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(Int x, Int y)\n"
                                                             "  x * y\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Multiplication Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, DivisionExpression) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(Int x, Int y)\n"
                                                             "  x / y\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Division Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, AssignmentExpression) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test()\n"
                                                             "  Int x\n"
                                                             "  Int y\n"
                                                             "  x = y\n"
                                                             "end\n");

    // root -> function
    ASSERT_EQ("Variable Declaration", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("x", node->childAtIndex(0)->name());
    ASSERT_EQ(NewDataType::Kind::Integer, node->childAtIndex(0)->dataType().kind());

    ASSERT_EQ("Variable Declaration", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("y", node->childAtIndex(1)->name());
    ASSERT_EQ(NewDataType::Kind::Integer, node->childAtIndex(1)->dataType().kind());

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(2));

    ASSERT_EQ("Assign Operator", opNode->nodeName());
    ASSERT_EQ("Local Variable", opNode->childAtIndex(0)->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());

    ASSERT_EQ("Local Variable", opNode->childAtIndex(1)->nodeName());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, SelfAssignmentExpression) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = a\n"
                                              "end\n");

    ASSERT_EQ("Assign Operator", node->childAtIndex(0)->nodeName());
}

TEST_F(NewParserTests_Operators, AdditionExpression) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(Int x, Int y)\n"
                                                             "  x + y\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Addition Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, SubtractionExpression) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(Int x, Int y)\n"
                                                             "  x - y\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Subtraction Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, ParenthesisExpression) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(Int x, Int y)\n"
                                                             "  x = (x - y) * y\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Assign Operator", opNode->nodeName());

    opNode = dynamic_cast<OperatorNode*>(opNode->childAtIndex(1));

    ASSERT_EQ("Multiplication Operator", opNode->nodeName());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());

    opNode = dynamic_cast<OperatorNode*>(opNode->childAtIndex(0));

    ASSERT_EQ("Subtraction Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, DereferenceOperator) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(*Int x)\n"
                                                             "  *x\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Dereference Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
}

TEST_F(NewParserTests_Operators, AddressOfOperator) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(Int x)\n"
                                                             "  &x\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Address Of Operator", opNode->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0))->name());
}

TEST_F(NewParserTests_Operators, AssignToDereference) {
    FunctionDefinitionNode* node = this->parseSingleFunction("def test(*Int x, *Int y)\n"
                                                             "  *x = *y\n"
                                                             "end\n");

    OperatorNode* opNode = dynamic_cast<OperatorNode*>(node->childAtIndex(0));

    ASSERT_EQ("Assign Operator", opNode->nodeName());

    ASSERT_EQ("Dereference Operator", opNode->childAtIndex(0)->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(opNode->childAtIndex(0)->childAtIndex(0))->name());

    ASSERT_EQ("Dereference Operator", opNode->childAtIndex(1)->nodeName());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(opNode->childAtIndex(1)->childAtIndex(0))->name());
}

TEST_F(NewParserTests_Operators, MemberAccessOperator) {
    ASTNode* node = this->parseNodeWithBodies("struct MyStruct\n"
                                              "  Int a\n"
                                              "end\n"
                                              "def test(MyStruct value, Int b)\n"
                                              "  value.a = b\n"
                                              "end\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("b", dynamic_cast<VariableNode*>(node->childAtIndex(1))->name());

    node = node->childAtIndex(0);
    ASSERT_EQ("Member Access Operator", node->nodeName());
    ASSERT_EQ("a", dynamic_cast<MemberAccessNode*>(node)->name());
    ASSERT_FALSE(dynamic_cast<MemberAccessNode*>(node)->indirect());

    node = node->childAtIndex(0);
    ASSERT_EQ("Local Variable", node->nodeName());
    ASSERT_EQ("value", dynamic_cast<VariableNode*>(node)->name());
}

TEST_F(NewParserTests_Operators, IndirectMemberAccessOperator) {
    ASTNode* node = this->parseNodeWithBodies("struct MyStruct\n"
                                              "  Int a\n"
                                              "end\n"
                                              "def test(*MyStruct value, Int b)\n"
                                              "  value->a = b\n"
                                              "end\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("b", dynamic_cast<VariableNode*>(node->childAtIndex(1))->name());

    node = node->childAtIndex(0);
    ASSERT_EQ("Member Access Operator", node->nodeName());
    ASSERT_EQ("a", dynamic_cast<MemberAccessNode*>(node)->name());
    ASSERT_TRUE(dynamic_cast<MemberAccessNode*>(node)->indirect());

    node = node->childAtIndex(0);
    ASSERT_EQ("Local Variable", node->nodeName());
    ASSERT_EQ("value", dynamic_cast<VariableNode*>(node)->name());
}

TEST_F(NewParserTests_Operators, AddressOfAndArrowPrecedence) {
    ASTNode* node = this->parseNodeWithBodies("struct MyStruct\n"
                                              "  Int x\n"
                                              "end\n"
                                              "def test(*MyStruct a, Int b)\n"
                                              "  b = &a->x\n"
                                              "end\n");

    node = node->childAtIndex(1)->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    ASSERT_EQ("Local Variable", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("b", dynamic_cast<VariableNode*>(node->childAtIndex(0))->name());

    node = node->childAtIndex(1);
    ASSERT_EQ("Address Of Operator", node->nodeName());

    node = node->childAtIndex(0);
    ASSERT_EQ("Member Access Operator", node->nodeName());
    ASSERT_EQ("x", dynamic_cast<MemberAccessNode*>(node)->name());

    node = node->childAtIndex(0);
    ASSERT_EQ("Local Variable", node->nodeName());
    ASSERT_EQ("a", dynamic_cast<VariableNode*>(node)->name());
}

TEST_F(NewParserTests_Operators, IndexerOperator) {
    ASTNode* node = this->parseSingleFunction("def test([5]Int x, Int y)\n"
                                              "  x[y] = y\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(node->childAtIndex(1))->name());

    node = node->childAtIndex(0);
    ASSERT_EQ("Indexer Operator", node->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("x", dynamic_cast<VariableNode*>(node->childAtIndex(0))->name());
    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("y", dynamic_cast<VariableNode*>(node->childAtIndex(1))->name());
}

TEST_F(NewParserTests_Operators, EqualOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int x)\n"
                                              "  x == x\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Equal Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, NotEqualOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int x)\n"
                                              "  x != x\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Not-Equal Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, TernaryConditionalOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int a, Int b, Int c)\n"
                                              "  a = (a == b) ? b : c\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Ternary Conditional Operator", node->nodeName());

    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("b", dynamic_cast<VariableNode*>(node->childAtIndex(1))->name());
    ASSERT_EQ("Local Variable", node->childAtIndex(2)->nodeName());
    ASSERT_EQ("c", dynamic_cast<VariableNode*>(node->childAtIndex(2))->name());
}

TEST_F(NewParserTests_Operators, TernaryCompareAndSwapOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int a, Int b, Int c)\n"
                                              "  a cas b : c\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("CAS Operator", node->nodeName());

    ASSERT_EQ("Local Variable", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("a", dynamic_cast<VariableNode*>(node->childAtIndex(0))->name());
    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
    ASSERT_EQ("b", dynamic_cast<VariableNode*>(node->childAtIndex(1))->name());
    ASSERT_EQ("Local Variable", node->childAtIndex(2)->nodeName());
    ASSERT_EQ("c", dynamic_cast<VariableNode*>(node->childAtIndex(2))->name());
}

TEST_F(NewParserTests_Operators, CompoundAdditionAssignmentOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int a, Int b)\n"
                                              "  a += b\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Add-Assign Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, CompoundBitwiseOrAssignmentOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int a, Int b)\n"
                                              "  a |= b\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Bitwise Or-Assign Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, PrecedenceTest) {
    ASTNode* node = this->parseSingleFunction("def test(Int x, Int y, Int z)\n"
                                              "  x = x * y + z\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    // addition
    node = node->childAtIndex(1);
    ASSERT_EQ("Addition Operator", node->nodeName());

    // multiplication
    node = node->childAtIndex(0);
    ASSERT_EQ("Multiplication Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, OrAssignmentOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int a, Int b)\n"
                                              "  a ||= b\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Or-Assign Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, UnaryMinusOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = -a\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Unary Minus Operator", node->nodeName());
}

TEST_F(NewParserTests_Operators, UnaryNotOperator) {
    ASTNode* node = this->parseSingleFunction("def test(Bool a)\n"
                                              "  a = !a\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Not Operator", node->nodeName());
}
