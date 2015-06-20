#include "CCodeGenTestsBase.h"

class CCodeGenTests_Tuples : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Tuples, FunctionDefinitionWithMultipleReturns) {
    Three::CCodeGenVisitor* visitor = this->visit("def test() -> (Int x, Int y)\nend\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct {\n"
              "    const int x;\n"
              "    const int y;\n"
              "} const_int_const_int_tuple_t;\n"
              "const const_int_const_int_tuple_t test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("const const_int_const_int_tuple_t test(void) {\n"
              "    int x;\n"
              "    int y;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Tuples, FunctionDefinitionWithMultipleReturnsWithoutNames) {
    Three::CCodeGenVisitor* visitor = this->visit("def test() -> (Int, Int)\nend\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct {\n"
              "    const int member_1;\n"
              "    const int member_2;\n"
              "} const_int_const_int_tuple_t;\n"
              "const const_int_const_int_tuple_t test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("const const_int_const_int_tuple_t test(void) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Tuples, CallFunctionWithMultipleReturns) {
    Three::CCodeGenVisitor* visitor = this->visit("def foo() -> (Int a, Int b)\n"
                                                  "  return 1, 2\n"
                                                  "end\n"
                                                  "def test()\n"
                                                  "  Int! a\n"
                                                  "  Int! b\n"
                                                  "  a, b = foo()\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct {\n"
              "    const int a;\n"
              "    const int b;\n"
              "} const_int_const_int_tuple_t;\n"
              "const const_int_const_int_tuple_t foo(void);\n"
              "void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("const const_int_const_int_tuple_t foo(void) {\n"
              "    int a;\n"
              "    int b;\n"
              "    return (const const_int_const_int_tuple_t){1, 2};\n"
              "}\n\n"
              "void test(void) {\n"
              "    int a;\n"
              "    int b;\n"
              "    const const_int_const_int_tuple_t tmp_return = foo();\n"
              "    a = tmp_return.a;\n"
              "    b = tmp_return.b;\n"
              "}\n\n", visitor->bodyString());
}
