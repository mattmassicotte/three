#include "ParserTestBase.h"

class ParserTest_Operators : public ParserTestBase {
};

TEST_F(ParserTest_Operators, AssignmentExpression) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt x\nx = 42\nend\n");

    Three::OperatorNode* opNode = dynamic_cast<Three::OperatorNode*>(node->childAtIndex(0)->childAtIndex(1));

    ASSERT_OPERATOR("=", opNode);
    ASSERT_VARIABLE_NODE("Int",  0, "x", opNode->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(42, opNode->childAtIndex(1));
}

TEST_F(ParserTest_Operators, ComplexExpression) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt x\nx = 42 * (5 + 1)\nend\n");
    node = node->childAtIndex(0);

    ASSERT_VARIABLE_DECLARATION("Int", 0, "x", node->childAtIndex(0));

    Three::OperatorNode* opNode = dynamic_cast<Three::OperatorNode*>(node->childAtIndex(1));

    ASSERT_OPERATOR("=", opNode);
    ASSERT_VARIABLE_NODE("Int",  0, "x", opNode->childAtIndex(0));

    opNode = dynamic_cast<Three::OperatorNode*>(opNode->childAtIndex(1));
    ASSERT_OPERATOR("*", opNode);
    ASSERT_INTEGER_LITERAL_NODE(42, opNode->childAtIndex(0));

    opNode = dynamic_cast<Three::OperatorNode*>(opNode->childAtIndex(1));
    ASSERT_OPERATOR("+", opNode);
    ASSERT_INTEGER_LITERAL_NODE(5, opNode->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, opNode->childAtIndex(1));
}

TEST_F(ParserTest_Operators, DereferenceUnaryOperator) {
    Three::ASTNode* node;

    node = this->parse("def test(*Int a)\n*a\nend\n");
    node = node->childAtIndex(0);

    ASSERT_OPERATOR("*", node->childAtIndex(0));
}

TEST_F(ParserTest_Operators, DereferenceUnaryOperatorAsLValue) {
    Three::ASTNode* node;

    node = this->parse("def test(*Int a)\n*a = null\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_OPERATOR("*", node->childAtIndex(0));
    ASSERT_EQ(1, node->childAtIndex(0)->childCount());
}

TEST_F(ParserTest_Operators, MoreComplexUnaryOperatorAsLValue) {
    Three::ASTNode* node;

    node = this->parse("def test(*Int a)\n***a = &*a\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_OPERATOR("*", node->childAtIndex(0));
    ASSERT_EQ(1, node->childAtIndex(0)->childCount());
}

TEST_F(ParserTest_Operators, AddressOfUnaryOperator) {
    Three::ASTNode* node;

    node = this->parse("def test(*Int a)\na = &a\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_OPERATOR("&", node->childAtIndex(1));
    ASSERT_EQ(1, node->childAtIndex(1)->childCount());
}

TEST_F(ParserTest_Operators, AddressOfPrecedence) {
    Three::ASTNode* node;

    node = this->parse("struct MyStruct\nInt x\nend\ndef test(*MyStruct a, Int b)\nb = &a->x\nend\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "b", node->childAtIndex(0));

    node = node->childAtIndex(1);
    ASSERT_OPERATOR("&", node);
    ASSERT_EQ("MemberAccess", node->childAtIndex(0)->nodeName());
}

TEST_F(ParserTest_Operators, SizeofOperator) {
    Three::ASTNode* node;

    node = this->parse("def test(Int x)\nx = sizeof(Int)\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_EQ("MetaFunction", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTest_Operators, MemberAccessOperator) {
    Three::ASTNode* node;

    node = this->parse("struct MyStruct\nInt a\nend\ndef test(MyStruct value)\n value.a = 0\nend\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));

    Three::MemberAccessNode* memberAccess = dynamic_cast<Three::MemberAccessNode*>(node->childAtIndex(0));
    ASSERT_EQ("MemberAccess", memberAccess->nodeName());
    ASSERT_EQ("a", memberAccess->memberName());
    ASSERT_TRUE(!memberAccess->indirect());
    ASSERT_VARIABLE_NODE("MyStruct",  0, "value", memberAccess->childAtIndex(0));
}

TEST_F(ParserTest_Operators, IndirectMemberAccessOperator) {
    Three::ASTNode* node;

    node = this->parse("struct MyStruct\nInt a\nend\ndef test(*MyStruct value)\n value->a = 0\nend\n");
    node = node->childAtIndex(1)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));

    Three::MemberAccessNode* memberAccess = dynamic_cast<Three::MemberAccessNode*>(node->childAtIndex(0));
    ASSERT_EQ("MemberAccess", memberAccess->nodeName());
    ASSERT_EQ("a", memberAccess->memberName());
    ASSERT_TRUE(memberAccess->indirect());
    ASSERT_VARIABLE_NODE("MyStruct",  1, "value", memberAccess->childAtIndex(0));
}

TEST_F(ParserTest_Operators, IndexerOperator) {
    Three::ASTNode* node;

    node = this->parse("def test(Int x)\nx[5] = 0\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("=", node);
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));

    Three::IndexerNode* indexer = dynamic_cast<Three::IndexerNode*>(node->childAtIndex(0));
    ASSERT_EQ("Indexer", indexer->nodeName());
    ASSERT_VARIABLE_NODE("Int", 0, "x", indexer->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(5, indexer->childAtIndex(1));
}

TEST_F(ParserTest_Operators, NotEqualOperator) {
    Three::ASTNode* node;

    node = this->parse("def test(Int x)\nx != 0\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("!=", node);
    ASSERT_VARIABLE_NODE("Int", 0, "x", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));
}

TEST_F(ParserTest_Operators, TernaryConditionalOperator) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt a\na = (a == 0) ? 1 : 2\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_OPERATOR("=", node);
    ASSERT_EQ(2, node->childCount());
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));

    node = node->childAtIndex(1);
    ASSERT_OPERATOR("?", node);
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
    ASSERT_INTEGER_LITERAL_NODE(2, node->childAtIndex(2));

    node = node->childAtIndex(0);
    ASSERT_OPERATOR("==", node);
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(0, node->childAtIndex(1));
}

TEST_F(ParserTest_Operators, TernaryCompareAndSwapOperator) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt a\na cas 1 : 2\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_OPERATOR("cas", node);
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
    ASSERT_INTEGER_LITERAL_NODE(2, node->childAtIndex(2));
}

TEST_F(ParserTest_Operators, CompoundAdditionAssignmentOperator) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt a\na += 1\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_OPERATOR("+=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}

TEST_F(ParserTest_Operators, CompoundBitwiseOrAssignmentOperator) {
    Three::ASTNode* node;

    node = this->parse("def test()\nInt a\na |= 1\nend\n");
    node = node->childAtIndex(0)->childAtIndex(1);

    ASSERT_OPERATOR("|=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "a", node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}

TEST_F(ParserTest_Operators, PrecedenceTest) {
    Three::ASTNode* node;

    node = this->parse("def test(Int x)\nx = 4 * 5 + 1\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    // assignment
    ASSERT_OPERATOR("=", node);
    ASSERT_VARIABLE_NODE("Int",  0, "x", node->childAtIndex(0));

    // addition
    node = node->childAtIndex(1);
    ASSERT_OPERATOR("+", node);
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));

    // multiplication
    node = node->childAtIndex(0);
    ASSERT_OPERATOR("*", node);
    ASSERT_INTEGER_LITERAL_NODE(4, node->childAtIndex(0));
    ASSERT_INTEGER_LITERAL_NODE(5, node->childAtIndex(1));
}

TEST_F(ParserTest_Operators, OrAssignmentOperator) {
    Three::ASTNode* node;

    node = this->parse("def test(Int a)\na ||= 1\nend\n");
    node = node->childAtIndex(0)->childAtIndex(0);

    ASSERT_OPERATOR("||=", node);
    ASSERT_INTEGER_LITERAL_NODE(1, node->childAtIndex(1));
}
