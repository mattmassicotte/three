#include "CCodeGenTestsBase.h"

class CCodeGenTests_Metafunctions : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Metafunctions, Sizeof) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  x = sizeof(Int!)\n"
                                                  "end\n");

    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = sizeof(int);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, Cast) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  Int! x = cast(Int!, 5)\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = (int)(5);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, Varargs) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Vararg ap)\n"
                                                  "  Int! a = nextarg(Int!, ap)\n"
                                                  "end\n");

    EXPECT_EQ("void test(va_list ap);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(va_list ap) {\n"
              "    int a = va_arg(ap, int);\n"
              "}\n\n", visitor->bodyString());
}
