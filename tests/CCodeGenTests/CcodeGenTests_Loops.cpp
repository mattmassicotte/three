#include "CCodeGenTestsBase.h"

class CCodeGenTests_Loops : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Loops, InfiniteLoop) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  loop\n"
                                                  "  end\n"
                                                  "end\n");


    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n    for (;;) {\n    }\n}\n\n", visitor->bodyString()); 
}

TEST_F(CCodeGenTests_Loops, ConditionLoop) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int x)\n"
                                                  "  loop x > 0\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n    while (x > 0) {\n    }\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Loops, ConditionLoopWithStatement) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int x)\n"
                                                  "  loop x > 0\n"
                                                  "    x += 1\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    while (x > 0) {\n"
              "        x += 1;\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Loops, AfterConditionLoop) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int x)\n"
                                                  "  loop:after x > 0\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n    do {\n    } while (x > 0);\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Loops, ForLoop) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  for (Int i = 0; i < 10; i += 1)\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n    for (int i = 0; i < 10; i += 1) {\n    }\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Loops, RangedForLoop) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  for (Int i in 0:10)\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n    for (int i = 0; (i < 10) && (0 < 10); ++i) {\n    }\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Loops, BreakStatement) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  loop\n"
                                                  "    break\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    for (;;) {\n"
              "        break;\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Loops, ContinueStatement) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  loop\n"
                                                  "    continue\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    for (;;) {\n"
              "        continue;\n"
              "    }\n"
              "}\n\n", visitor->bodyString());
}
