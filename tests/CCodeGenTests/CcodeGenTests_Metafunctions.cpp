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

TEST_F(CCodeGenTests_Metafunctions, Cardinalityof) {
    Three::CCodeGenVisitor* visitor = this->visit("def test([5]Int x)\n"
                                                  "  Int y = cardinalityof(x)\n"
                                                  "end\n");

    EXPECT_EQ("void test(const int[5] x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(const int[5] x) {\n"
              "    const int y = sizeof(x);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, Alignof) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  x = alignof(Int!)\n"
                                                  "end\n");

    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = alignof(int);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, Typeof) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\n"
                                                  "  typeof(Int!)\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    typeof(int);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Metafunctions, Offsetof) {
    Three::CCodeGenVisitor* visitor = this->visit("struct MyStruct\n"
                                                  "  Int a\n"
                                                  "end\n"
                                                  "def test()\n"
                                                  "  offsetof(MyStruct, a)\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct MyStruct {\n"
              "    int a;\n"
              "} MyStruct;\n\n"
              "void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    offsetof(const MyStruct,a);\n"
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
