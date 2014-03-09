#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_CInterop : public ParserTestBase {
};

TEST_F(CCodeGenTests_CInterop, UsingFunctionFromCHeader) {
    ASTNode* node = this->parse("include \"stdio.h\"\n\ndef test()\nprintf(\"hello world\")\nend\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n    printf(\"hello world\");\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_CInterop, UsingConstantMacroFromCHeader) {
    ASTNode* node = this->parse("include \"stdio.h\"\n\n"
                                "def test()\n"
                                "  Int x = EOF\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(void) {\n    int x = EOF;\n}\n\n", visitor.bodyString());
}
