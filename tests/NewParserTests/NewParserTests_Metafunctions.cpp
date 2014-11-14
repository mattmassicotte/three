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

TEST_F(ParserTests_Metafunctions, DISABLED_Alignof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  alignof(Int)\n"
                                                     "end\n");

    std::cout << node->recursiveStr() << std::endl;
    ASSERT_TRUE(false);
}

TEST_F(ParserTests_Metafunctions, DISABLED_Offsetof) {
    Three::ASTNode* node = this->parseNodeWithBodies("struct MyStruct\n"
                                                     "  a\n"
                                                     "end\n"
                                                     "def test()\n"
                                                     "  offsetof(MyStruct, a)\n"
                                                     "end\n");

    std::cout << node->recursiveStr() << std::endl;
    ASSERT_TRUE(false);
}

TEST_F(ParserTests_Metafunctions, DISABLED_Nameof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  nameof(Int)\n"
                                                     "end\n");
    
    std::cout << node->recursiveStr() << std::endl;
    ASSERT_TRUE(false);
}

TEST_F(ParserTests_Metafunctions, DISABLED_Cardinalityof) {
    Three::ASTNode* node = this->parseSingleFunction("def test([5]Int array)\n"
                                                     "  cardinalityof(array)\n"
                                                     "end\n");
    
    std::cout << node->recursiveStr() << std::endl;
    ASSERT_TRUE(false);
}

TEST_F(ParserTests_Metafunctions, DISABLED_abiof) {
    Three::ASTNode* node = this->parseSingleFunction("def test()\n"
                                                     "  abiof(test)\n"
                                                     "end\n");
    
    std::cout << node->recursiveStr() << std::endl;
    ASSERT_TRUE(false);
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
