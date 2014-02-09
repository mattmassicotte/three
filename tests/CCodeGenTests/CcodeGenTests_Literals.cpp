#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Literals : public ParserTestBase {
};

TEST_F(CCodeGenTests_Literals, LiteralNull) {
    ASTNode* node = this->parse("def test(*Int x)\n"
                                "  x = null\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int* x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int* x) {\n"
              "    x = NULL;\n"
              "}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Literals, BooleanLiteral) {
    ASTNode* node = this->parse("def test(Bool x)\n"
                                "  x = true\n"
                                "  x = false\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(bool x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(bool x) {\n"
              "    x = true;\n"
              "    x = false;\n"
              "}\n\n", visitor.bodyString());
}
