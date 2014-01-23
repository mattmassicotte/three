#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Loops : public ParserTestBase {
};

TEST_F(CCodeGenTests_Loops, InfiniteLoop) {
    ASTNode* node = this->parse("def test()\n"
                                "  loop\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n    for (;;) {\n    }\n}\n\n", visitor.bodyString()); 
}

TEST_F(CCodeGenTests_Loops, ConditionLoop) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  loop x > 0\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n    while (x > 0) {\n    }\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Loops, ConditionLoopWithStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  loop x > 0\n"
                                "    x += 1\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    while (x > 0) {\n"
              "        x += 1;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Loops, AfterConditionLoop) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  loop:after x > 0\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n    do {\n    } while (x > 0);\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Loops, ForLoop) {
    ASTNode* node = this->parse("def test()\n"
                                "  for (Int i = 0; i < 10; i += 1)\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n    for (int i = 0; i < 10; i += 1) {\n    }\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Loops, RangedForLoop) {
    ASTNode* node = this->parse("def test()\n"
                                "  for (Int i in 0:10)\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n    for (int i = 0; (i < 10) && (0 < 10); ++i) {\n    }\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Loops, BreakStatement) {
    ASTNode* node = this->parse("def test()\n"
                                "  loop\n"
                                "    break\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    for (;;) {\n"
              "        break;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Loops, ContinueStatement) {
    ASTNode* node = this->parse("def test()\n"
                                "  loop\n"
                                "    continue\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    for (;;) {\n"
              "        continue;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}
