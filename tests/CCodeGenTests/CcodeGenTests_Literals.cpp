#include "CCodeGenTestsBase.h"

class CCodeGenTests_Literals : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Literals, LiteralNull) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(*Int x)\n"
                                                  "  x = null\n"
                                                  "end\n");

    EXPECT_EQ("void test(int* x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int* x) {\n"
              "    x = NULL;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Literals, BooleanLiteral) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Bool x)\n"
                                                  "  x = true\n"
                                                  "  x = false\n"
                                                  "end\n");

    EXPECT_EQ("void test(bool x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(bool x) {\n"
              "    x = true;\n"
              "    x = false;\n"
              "}\n\n", visitor->bodyString());
}
