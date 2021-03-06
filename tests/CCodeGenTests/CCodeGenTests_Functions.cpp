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
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int a, Int b) -> *Int\nend\n");

    EXPECT_EQ("const int* const test(const int a, const int b);\n", visitor->internalHeaderString());
    EXPECT_EQ("const int* const test(const int a, const int b) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionDefinitionWithNamedReturn) {
    Three::CCodeGenVisitor* visitor = this->visit("def test() -> Int x\nend\n");

    EXPECT_EQ("const int test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("const int test(void) {\n"
              "    int x;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionWithClosureArgument) {
    Three::CCodeGenVisitor* visitor = this->visit("def test({} closure)\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void test(const three_closure_t closure);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(const three_closure_t closure) {\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, NamespacedFunction) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace Foo\n"
                                                  "  def test(Int x)\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void Foo_3_test(const int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void Foo_3_test(const int x) {\n"
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
    EXPECT_EQ("void foo(void) {\n}\n\nvoid test(void) {\n    void* const ptr = (&foo);\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, MethodInvocation) {
    Three::CCodeGenVisitor* visitor = this->visit("def Int.foo(Int b)\n"
                                                  "end\n"
                                                  "def test(*Int i)\n"
                                                  "  i.foo(5)\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("void Int_3_foo(const int* const self, const int b);\n"
              "void test(const int* const i);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void Int_3_foo(const int* const self, const int b) {\n"
              "}\n\n"
              "void test(const int* const i) {\n"
              "    Int_3_foo(i, 5);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, NestedNamespacedFunction) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace One::Two\n"
                                                  "  def foo()\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void One_3_Two_3_foo(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void One_3_Two_3_foo(void) {\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, CallingNamespacedFunction) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace One::Two\n"
                                                  "  def foo()\n"
                                                  "  end\n"
                                                  "  def bar()\n"
                                                  "    foo()\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("void One_3_Two_3_foo(void);\n"
              "void One_3_Two_3_bar(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void One_3_Two_3_foo(void) {\n}\n\n"
              "void One_3_Two_3_bar(void) {\n"
              "    One_3_Two_3_foo();\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Functions, CallingNamespacedMethodOnType) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace One::Two\n"
                                                  "  struct Foo\n"
                                                  "  end\n"
                                                  "  def Foo.method()\n"
                                                  "  end\n"
                                                  "end\n"
                                                  "def test(*One::Two::Foo i)\n"
                                                  "  i.method()\n"
                                                  "end\n");

    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("typedef struct One_3_Two_3_Foo {\n"
              "} One_3_Two_3_Foo;\n\n"
              "void One_3_Two_3_Foo_3_method(const One_3_Two_3_Foo* const self);\n"
              "void test(const One_3_Two_3_Foo* const i);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void One_3_Two_3_Foo_3_method(const One_3_Two_3_Foo* const self) {\n"
              "}\n\n"
              "void test(const One_3_Two_3_Foo* const i) {\n"
              "    One_3_Two_3_Foo_3_method(i);\n"
              "}\n\n", visitor->bodyString());
}
