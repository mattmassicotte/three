#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Control : public ParserTestBase {
};

TEST_F(CCodeGenTests_Control, IfStatementWithElse) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  if x > 0\n"
                                "    x = 1\n"
                                "  else\n"
                                "    x = 2\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    if (x > 0) {\n"
              "        x = 1;\n"
              "    } else {\n"
              "        x = 2;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Control, IfStatementWithNonParenthesisConditional) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  if x\n"
                                "    x = 2\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    if (x) {\n"
              "        x = 2;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Control, SwitchStatement) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  switch x\n"
                                "  case 1\n"
                                "    x = 5\n"
                                "  case 2\n"
                                "    x = 6\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    if (x == 1) {\n"
              "        x = 5;\n"
              "    } else if (x == 2) {\n"
              "        x = 6;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Control, SwitchStatementWithElse) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  switch x\n"
                                "  case 1\n"
                                "    x = 5\n"
                                "  else\n"
                                "    x = 6\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    if (x == 1) {\n"
              "        x = 5;\n"
              "    } else {\n"
              "        x = 6;\n"
              "    }\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Control, Return) {
    ASTNode* node = this->parse("def test(; Int)\n"
                                "  return\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("int test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("int test(void) {\n    return;\n}\n\n", visitor.bodyString());
}


TEST_F(CCodeGenTests_Control, ReturnWithExpression) {
    ASTNode* node = this->parse("def test(; Int)\n"
                                "  return 1 + 1\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("int test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("int test(void) {\n    return 1 + 1;\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Control, FunctionWithEnsure) {
    ASTNode* node = this->parse("def cleanup()\n"
                                "end\n"
                                "def test()\n"
                                "  if true\n"
                                "    return\n"
                                "  end\n"
                                "ensure\n"
                                "  cleanup()\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void cleanup(void) {\n"
              "}\n\n"
              "void test(void) {\n"
              "    if (true) {\n"
              "        // ensure clause\n"
              "        cleanup();\n"
              "        return;\n"
              "    }\n"
              "    // ensure clause\n"
              "    cleanup();\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Control, FunctionWithEnsureReturningAValue) {
    ASTNode* node = this->parse("def cleanup()\n"
                                "end\n"
                                "def test(; Int)\n"
                                "  if true\n"
                                "    return 5 + 1\n"
                                "  end\n"
                                "  return 1\n"
                                "ensure\n"
                                "  cleanup()\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void cleanup(void) {\n"
              "}\n\n"
              "int test(void) {\n"
              "    if (true) {\n"
              "        int tmp_return_value_1 = 5 + 1;\n"
              "        // ensure clause\n"
              "        cleanup();\n"
              "        return tmp_return_value_1;\n"
              "    }\n"
              "    int tmp_return_value_2 = 1;\n"
              "    // ensure clause\n"
              "    cleanup();\n"
              "    return tmp_return_value_2;\n"
              "}\n\n", visitor.bodyString());
}