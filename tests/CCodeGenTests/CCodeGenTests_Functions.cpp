#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Functions : public ParserTestBase {
};

TEST_F(CCodeGenTests_Functions, FunctionDefinition) {
    ASTNode* node = this->parse("def test()\nend\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Functions, PrivateFunctionDefinition) {
    ASTNode* node = this->parse("private\n"
                                "def test()\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(void);\n", visitor.declarationsString());
    EXPECT_EQ("", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void test(void) {\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionDefinitionWithArguments) {
    ASTNode* node = this->parse("def test(Int a, Int b; *Int)\nend\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("int* test(int a, int b);\n", visitor.internalHeaderString());
    EXPECT_EQ("int* test(int a, int b) {\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionWithClosureArgument) {
    ASTNode* node = this->parse("def test({} closure)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("", visitor.declarationsString());
    EXPECT_EQ("void test(three_closure_t closure);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void test(three_closure_t closure) {\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Functions, NamespacedFunction) {
    ASTNode* node = this->parse("namespace Foo\n"
                                "  def test(Int x)\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void Foo_3_test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void Foo_3_test(int x) {\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Functions, FunctionReferences) {
    ASTNode* node = this->parse("def foo()\n"
                                "end\n"
                                "def test()\n"
                                "  *Void ptr = &foo\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void foo(void);\nvoid test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void foo(void) {\n}\n\nvoid test(void) {\n    void* ptr = (&foo);\n}\n\n", visitor.bodyString());
}
