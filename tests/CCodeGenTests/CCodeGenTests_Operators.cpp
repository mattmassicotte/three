#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Operators : public ParserTestBase {
};

TEST_F(CCodeGenTests_Operators, MemberAccess) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  Natural:32 a\n"
                                "end\n"
                                "def test(Foo f)\n"
                                "  f.a = 0\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("typedef struct Foo {\n    uint32_t a;\n} Foo;\n\nvoid test(Foo f);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void test(Foo f) {\n    f.a = 0;\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Operators, FunctionCallOperator) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  *(Int) fn\n"
                                "end\n"
                                "def test(Foo f)\n"
                                "  f.fn(1)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("typedef struct Foo {\n"
              "    void (*fn)(int);\n"
              "} Foo;\n\n"
              "void test(Foo f);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void test(Foo f) {\n    f.fn(1);\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Operators, StructMemberWithUnaryOperators) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  Int a\n"
                                "end\n"
                                "def test(**Foo f)\n"
                                "  f = &(*f)->a\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("typedef struct Foo {\n    int a;\n} Foo;\n\nvoid test(Foo** f);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(Foo** f) {\n    f = (&(*f)->a);\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Operators, TernaryConditionalOperator) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = x ? 0 : 1\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = x ? 0 : 1;\n"
              "}\n\n", visitor.bodyString()); 
}

TEST_F(CCodeGenTests_Operators, CompoundAssignOperator) {
    ASTNode* node = this->parse("def test(Int a)\n"
                                "  a |= 5\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int a);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int a) {\n    a |= 5;\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Operators, Indexer) {
    ASTNode* node = this->parse("def test(*Int a)\n"
                                "  a[0] = 5\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int* a);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int* a) {\n    a[0] = 5;\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Operators, UnaryMinus) {
    ASTNode* node = this->parse("def test(Int a)\n"
                                "  a = -a\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int a);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int a) {\n    a = (-a);\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Operators, UnaryNot) {
    ASTNode* node = this->parse("def test(Bool a)\n"
                                "  a = !a\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(bool a);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(bool a) {\n    a = (!a);\n}\n\n", visitor.bodyString());
}
