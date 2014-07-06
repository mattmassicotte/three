#include "CCodeGenTestsBase.h"

class CCodeGenTests_Tuples : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Tuples, FunctionDefinitionWithMultipleReturns) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(; Int a, Int b)\nend\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct {\n"
              "    int a;\n"
              "    int b;\n"
              "} int_int_tuple_t;\n"
              "int_int_tuple_t test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("int_int_tuple_t test(void) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Tuples, CallFunctionWithMultipleReturns) {
    Three::CCodeGenVisitor* visitor = this->visit("def foo(; Int a, Int b)\n"
                                                  "  return 1, 2\n"
                                                  "end\n"
                                                  "def test()\n"
                                                  "  Int a\n"
                                                  "  Int b\n"
                                                  "  a, b = foo()\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct {\n"
              "    int a;\n"
              "    int b;\n"
              "} int_int_tuple_t;\n"
              "int_int_tuple_t foo(void);\n"
              "void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("int_int_tuple_t foo(void) {\n"
              "    return (int_int_tuple_t){1, 2};\n"
              "}\n\n"
              "void test(void) {\n"
              "    int a;\n"
              "    int b;\n"
              "    int_int_tuple_t tmp_return = foo();\n"
              "    a = tmp_return.a;\n"
              "    b = tmp_return.b;\n"
              "}\n\n", visitor->bodyString());
}
