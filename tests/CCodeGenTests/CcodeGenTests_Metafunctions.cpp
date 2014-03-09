#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Metafunctions : public ParserTestBase {
};

TEST_F(CCodeGenTests_Metafunctions, Sizeof) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = sizeof(Int)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = sizeof(int);\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, MetafuctionFromCIncludedFunction) {
    ASTNode* node = this->parse("include \"stdlib.h\"\n"
                                "def test()\n"
                                "  *Int x = malloc(sizeof(Int))\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void) {\n"
              "    int* x = malloc(sizeof(int));\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, Cast) {
    ASTNode* node = this->parse("def test()\n"
                                "  Int x = cast(Int, 5)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = (int)(5);\n"
              "}\n\n", visitor.bodyString());
}
