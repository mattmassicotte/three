#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Metafunctions : public ParserTestBase {
};

TEST_F(CCodeGenTests_Metafunctions, Sizeof) {
    ASTNode* node = this->parse("def test(Int x)\n"
                                "  x = sizeof(Int)\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int x);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int x) {\n"
              "    x = sizeof(int);\n"
              "}\n\n", visitor.bodyString());
}
