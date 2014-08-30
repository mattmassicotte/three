#include "../ParserTestsBase.h"

class ParserTests_Tuples : public ParserTestsBase {
};

TEST_F(ParserTests_Tuples, FunctionNamedReturnsAreLocal) {
    ASTNode* node = this->parseSingleFunction("def foo(; Int! x, Int y)\n"
                                              "  x = y\n"
                                              "  return x, y\n"
                                              "end\n");

    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(2, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTests_Tuples, ReturnTwoExpressions) {
    ASTNode* node = this->parseSingleFunction("def test(Int a; Int, Int)\n"
                                              "  return a + a, a\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Return", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Tuple", node->nodeName());
    ASSERT_EQ(2, node->dataType().subtypeCount());
    ASSERT_EQ("Addition Operator", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());

    ASSERT_EQ(NewDataType::Kind::Tuple, node->dataType().kind());
    ASSERT_EQ(2, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
    ASSERT_EQ(NewDataType::Kind::Integer, node->dataType().subtypeAtIndex(1).kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().subtypeAtIndex(1).access());
}

TEST_F(ParserTests_Tuples, ReturnTwoExpressionsWithMutability) {
    ASTNode* node = this->parseSingleFunction("def test(Int a; Int, Int!)\n"
                                              "  return a + a, a\n"
                                              "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Return", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Tuple", node->nodeName());
    ASSERT_EQ(2, node->dataType().subtypeCount());
    ASSERT_EQ("Addition Operator", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());

    ASSERT_EQ(NewDataType::Kind::Tuple, node->dataType().kind());
    ASSERT_EQ(2, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
    ASSERT_EQ(NewDataType::Kind::Integer, node->dataType().subtypeAtIndex(1).kind());
    ASSERT_EQ(NewDataType::Access::ReadWrite, node->dataType().subtypeAtIndex(1).access());
}

TEST_F(ParserTests_Tuples, ReturnTwoExpressionsTailingIf) {
    ASTNode* node = this->parseSingleFunction("def test(Int a; Int, Int)\n"
                                              "  return a + a, a if a\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("If", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Return", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    node = node->childAtIndex(0);
    ASSERT_EQ("Tuple", node->nodeName());
    ASSERT_EQ("Addition Operator", node->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", node->childAtIndex(1)->nodeName());
}

TEST_F(ParserTests_Tuples, DestructuringAssignment) {
    ASTNode* node = this->parseNodeWithBodies("def foo(; Int a, Int b)\n"
                                              "  return 1, 2\n"
                                              "end\n"
                                              "def test(Int a, Int b)\n"
                                              "  a, b = foo()\n"
                                              "end\n");

    ASSERT_EQ(2, node->childCount());
    EXPECT_EQ("Function Definition", node->childAtIndex(0)->nodeName());

    node = node->childAtIndex(1); // def test
    ASSERT_EQ("Function Definition", node->nodeName());
    ASSERT_EQ(1, node->childCount());

    ASTNode* opNode = node->childAtIndex(0);
    ASSERT_EQ("Destructured Assigment", opNode->nodeName());
    ASSERT_EQ(2, opNode->childCount());

    ASTNode* tupleNode = opNode->childAtIndex(0);
    ASSERT_EQ("Tuple", tupleNode->nodeName());
    ASSERT_EQ(2, tupleNode->childCount());
    ASSERT_EQ("Local Variable", tupleNode->childAtIndex(0)->nodeName());
    ASSERT_EQ("Local Variable", tupleNode->childAtIndex(1)->nodeName());

    ASSERT_EQ("Function Call Operator", opNode->childAtIndex(1)->nodeName());
}
