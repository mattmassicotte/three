#include "CCodeGenTestsBase.h"

class CCodeGenTests_CInterop : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_CInterop, UsingFunctionFromCHeader) {
    Three::CCodeGenVisitor* visitor = this->visit("include <stdio.h>\n\n"
                                                  "def test()\n"
                                                  "  printf(\"hello world\")\n"
                                                  "end\n");

    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    printf(\"hello world\");\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_CInterop, DISABLED_UsingConstantMacroFromCHeader) {
    Three::CCodeGenVisitor* visitor = this->visit("include <stdio.h>\n\n"
                                                  "def test()\n"
                                                  "  Int x = EOF\n"
                                                  "end\n");


    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    int x = EOF;\n"
              "}\n\n", visitor->bodyString());
}
