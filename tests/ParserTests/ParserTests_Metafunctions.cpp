#include "../ParserTestsBase.h"
#include "compiler/constructs/DataType.h"

class ParserTests_Metafunctions : public ParserTestsBase {
};

TEST_F(ParserTests_Metafunctions, Sizeof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  sizeof(Int)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Sizeof Metafunction", node->nodeName());

    Three::SizeofNode* metaFn = dynamic_cast<Three::SizeofNode*>(node);
    EXPECT_EQ(DataType::Kind::Integer, metaFn->dataTypeArgument.kind());
    EXPECT_EQ(0, metaFn->childCount());
}

TEST_F(ParserTests_Metafunctions, Alignof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  alignof(Int)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Alignof Metafunction", node->nodeName());

    auto metaFn = dynamic_cast<Three::AlignofNode*>(node);
    EXPECT_EQ(DataType::Kind::Integer, metaFn->dataTypeArgument.kind());
    EXPECT_EQ(0, metaFn->childCount());
}

TEST_F(ParserTests_Metafunctions, Offsetof) {
    Three::ASTNode* node = this->parseNodeWithBodies("struct MyStruct\n"
                                                     "  a\n"
                                                     "end\n"
                                                     "def test()\n"
                                                     "  offsetof(MyStruct, a)\n"
                                                     "end\n");

    ASSERT_EQ(2, node->childCount());
    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    ASSERT_EQ(1, node->childCount());
    node = node->childAtIndex(0);
    ASSERT_EQ("Offsetof Metafunction", node->nodeName());
    
    auto metaFn = dynamic_cast<Three::OffsetofNode*>(node);
    EXPECT_EQ(DataType::Kind::Structure, metaFn->dataTypeArgument.kind());
    EXPECT_EQ("a", metaFn->memberName);
    EXPECT_EQ(0, metaFn->childCount());
}

TEST_F(ParserTests_Metafunctions, Nameof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  nameof(Int)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Nameof Metafunction", node->nodeName());

    auto metaFn = dynamic_cast<Three::NameofNode*>(node);
    EXPECT_EQ(DataType::Kind::Integer, metaFn->dataTypeArgument.kind());
    EXPECT_EQ(0, metaFn->childCount());
}

TEST_F(ParserTests_Metafunctions, Typeof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  typeof(Int)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Typeof Metafunction", node->nodeName());

    auto metaFn = dynamic_cast<Three::TypeofNode*>(node);
    EXPECT_EQ(DataType::Kind::Integer, metaFn->dataTypeArgument.kind());
    EXPECT_EQ(0, metaFn->childCount());
}

TEST_F(ParserTests_Metafunctions, Cardinalityof) {
    Three::ASTNode* node = this->parseSingleFunction("def test([5]Int array)\n"
                                                     "  cardinalityof(array)\n"
                                                     "end\n");
    node = node->childAtIndex(0);
    ASSERT_EQ("Cardinalityof Metafunction", node->nodeName());

    auto metaFn = dynamic_cast<Three::CardinalityofNode*>(node);
    ASSERT_EQ(1, metaFn->childCount());
    EXPECT_EQ("Local Variable", metaFn->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Metafunctions, Containerof) {
    Three::ASTNode* node = this->parseNodeWithBodies("struct MyStruct\n"
                                                     "  Int a\n"
                                                     "  Int b\n"
                                                     "end\n"
                                                     "def test(*Int ptrB)\n"
                                                     "  containerof(ptrB, MyStruct, b)\n"
                                                     "end\n");

    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(2, node->childCount());
    node = node->childAtIndex(1);
    ASSERT_EQ("Function Definition", node->nodeName());

    ASSERT_EQ(1, node->childCount());
    node = node->childAtIndex(0);
    ASSERT_EQ("Containerof Metafunction", node->nodeName());
    
    auto metaFn = dynamic_cast<Three::ContainerofNode*>(node);
    EXPECT_EQ(DataType::Kind::Structure, metaFn->dataTypeArgument.kind());
    EXPECT_EQ("b", metaFn->memberName);
    ASSERT_EQ(1, metaFn->childCount());
    EXPECT_EQ("Local Variable", metaFn->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Metafunctions, Abiof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  abiof(test)\n"
                                                     "end\n");

    ASSERT_EQ(1, node->childCount());
    node = node->childAtIndex(0);
    ASSERT_EQ("ABIof Metafunction", node->nodeName());
    
    auto metaFn = dynamic_cast<Three::ABIofNode*>(node);
    ASSERT_EQ(1, metaFn->childCount());
    EXPECT_EQ("Function Variable", metaFn->childAtIndex(0)->nodeName());
    EXPECT_EQ("test", metaFn->childAtIndex(0)->name());
}

TEST_F(ParserTests_Metafunctions, Cast) {
    Three::ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                                     "  a = cast(Int, 5)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Cast Metafunction", node->nodeName());

    Three::CastNode* cast = dynamic_cast<Three::CastNode*>(node);
    EXPECT_EQ(DataType::Kind::Integer, cast->dataTypeArgument.kind());
    ASSERT_EQ(1, cast->childCount());
    EXPECT_EQ("Integer Literal", cast->childAtIndex(0)->nodeName());
}

TEST_F(ParserTests_Metafunctions, Nextarg) {
    Three::ASTNode* node = this->parseSingleFunction("def test(Int a, Vararg ap)\n"
                                                     "  a = nextarg(Int, ap)\n"
                                                     "end\n");

    node = node->childAtIndex(0);
    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Nextarg Metafunction", node->nodeName());

    Three::NextargNode* nextarg = dynamic_cast<Three::NextargNode*>(node);
    EXPECT_EQ(DataType::Kind::Integer, nextarg->dataTypeArgument.kind());
    ASSERT_EQ(1, nextarg->childCount());
    EXPECT_EQ("Local Variable", nextarg->childAtIndex(0)->nodeName());
}
