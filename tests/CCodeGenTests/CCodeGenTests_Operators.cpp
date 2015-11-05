#include "CCodeGenTestsBase.h"

class CCodeGenTests_Operators : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Operators, MemberAccess) {
    Three::CCodeGenVisitor* visitor = this->visit("struct Foo\n"
                                                  "  Natural:32 a\n"
                                                  "end\n"
                                                  "def test(Foo! f)\n"
                                                  "  f.a = 0\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n"
              "    uint32_t a;\n"
              "} Foo;\n\n"
              "void test(Foo f);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(Foo f) {\n    f.a = 0;\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, IndirectMemberAccess) {
    Three::CCodeGenVisitor* visitor = this->visit("struct Foo\n"
                                                  "  Natural:32 a\n"
                                                  "end\n"
                                                  "def test(*Foo f)\n"
                                                  "  f->a = 0\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n"
              "    uint32_t a;\n"
              "} Foo;\n\n"
              "void test(const Foo* const f);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(const Foo* const f) {\n    f->a = 0;\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, NamespacedMemberAccess) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace Bar\n"
                                                  "  struct Foo\n"
                                                  "    Natural:32 a\n"
                                                  "  end\n"
                                                  "  def test(*Void p)\n"
                                                  "    Foo f = (*p)\n"
                                                  "    f.a = 0\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Bar_3_Foo {\n"
              "    uint32_t a;\n"
              "} Bar_3_Foo;\n\n"
              "void Bar_3_test(void* const p);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void Bar_3_test(void* const p) {\n"
              "    const Bar_3_Foo f = (*p);\n"
              "    f.a = 0;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, FunctionCallOperator) {
    Three::CCodeGenVisitor* visitor = this->visit("struct Foo\n"
                                                  "  *(Int) -> Void fn\n"
                                                  "end\n"
                                                  "def test(Foo f)\n"
                                                  "  f.fn(1)\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n"
              "    void (* fn)(const int);\n"
              "} Foo;\n\n"
              "void test(const Foo f);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void test(const Foo f) {\n    f.fn(1);\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, StructMemberWithUnaryOperators) {
    Three::CCodeGenVisitor* visitor = this->visit("struct Foo\n"
                                                  "  Int a\n"
                                                  "end\n"
                                                  "def test(*!*Foo f)\n"
                                                  "  f = &(*f)->a\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n"
              "    int a;\n"
              "} Foo;\n\n"
              "void test(const Foo* const* f);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(const Foo* const* f) {\n"
              "    f = (&(*f)->a);\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, TernaryConditionalOperator) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! x)\n"
                                                  "  x = x ? 0 : 1\n"
                                                  "end\n");

    EXPECT_EQ("void test(int x);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = x ? 0 : 1;\n"
              "}\n\n", visitor->bodyString()); 
}

TEST_F(CCodeGenTests_Operators, CompoundAssignOperator) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! a)\n"
                                                  "  a |= 5\n"
                                                  "end\n");

    EXPECT_EQ("void test(int a);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int a) {\n    a |= 5;\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, Indexer) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(*Int! a)\n"
                                                  "  a[0] = 5\n"
                                                  "end\n");

    EXPECT_EQ("void test(int* const a);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int* const a) {\n    a[0] = 5;\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, UnaryMinus) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int! a)\n"
                                                  "  a = -a\n"
                                                  "end\n");

    EXPECT_EQ("void test(int a);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(int a) {\n    a = (-a);\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Operators, UnaryNot) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Bool! a)\n"
                                                  "  a = !a\n"
                                                  "end\n");

    EXPECT_EQ("void test(bool a);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(bool a) {\n    a = (!a);\n}\n\n", visitor->bodyString());
}
