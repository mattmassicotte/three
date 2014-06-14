#include "CCodeGenTestsBase.h"

class CCodeGenTests_Functions : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Functions, FunctionDefinition) {
    Three::CCodeGenVisitor* visitor = this->visit("def test()\nend\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(void) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, PrivateFunctionDefinition) {
    Three::CCodeGenVisitor* visitor = this->visit("private\n"
                                                  "def test()\n"
                                                  "end\n");

    EXPECT_EQ("void test(void);\n", visitor->declarationsString());
    EXPECT_EQ("", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(void) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionDefinitionWithArguments) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int a, Int b; *Int)\nend\n");

    EXPECT_EQ("int* test(int a, int b);\n", visitor->internalHeaderString());
    EXPECT_EQ("int* test(int a, int b) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionWithClosureArgument) {
    Three::CCodeGenVisitor* visitor = this->visit("def test({} closure)\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void test(three_closure_t closure);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(three_closure_t closure) {\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, NamespacedFunction) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace Foo\n"
                                                  "  def test(Int x)\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void Foo_3_test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void Foo_3_test(int x) {\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionReferences) {
    Three::CCodeGenVisitor* visitor = this->visit("def foo()\n"
                                                  "end\n"
                                                  "def test()\n"
                                                  "  *Void ptr = &foo\n"
                                                  "end\n");

    EXPECT_EQ("void foo(void);\nvoid test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void foo(void) {\n}\n\nvoid test(void) {\n    void* ptr = (&foo);\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionDefinitionWithMultipleReturns) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(; Int a, Int b)\nend\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct {\n"
              "    int a;\n"
              "    int b;\n"
              "} test_returns;\n"
              "test_returns test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("test_returns test(void) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, MethodInvocation) {
    Three::CCodeGenVisitor* visitor = this->visit("def Int.foo(Int b)\n"
                                                  "end\n"
                                                  "def test(*Int i)\n"
                                                  "  i.foo(5)\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void Int_3_foo(int* self, int b);\n"
              "void test(int* i);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void Int_3_foo(int* self, int b) {\n"
              "}\n\n"
              "void test(int* i) {\n"
              "    Int_3_foo(i, 5);\n"
              "}\n\n", visitor->bodyString());
}
