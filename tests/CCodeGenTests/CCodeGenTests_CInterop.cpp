#include "CCodeGenTestsBase.h"

class CCodeGenTests_CInterop : public CCodeGenTestsBase {
    std::string testDataPath(const std::string& name) {
        return "/tmp/ThreeTestData/" + name;
    }
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

TEST_F(CCodeGenTests_CInterop, UsingConstantMacroFromCHeader) {
    Three::CCodeGenVisitor* visitor = this->visit("include <stdio.h>\n\n"
                                                  "def test()\n"
                                                  "  Int x = EOF\n"
                                                  "end\n");


    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    const int x = EOF;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_CInterop, StructureTypeDefinedInCHeader) {
    Three::CCodeGenVisitor* visitor = this->visit("include <stdio.h>\n\n"
                                                  "def test()\n"
                                                  "  *FILE! x = null\n"
                                                  "end\n");

    EXPECT_EQ("#include <stdio.h>\n\nvoid test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    FILE* const x = NULL;\n"
              "}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_CInterop, UsingTypedefTypeDefinedInCHeader) {
    Three::CCodeGenVisitor* visitor = this->visit("include \"/tmp/ThreeTestData/CSources/Typedefs.h\"\n\n"
                                                  "def test()\n"
                                                  "  pointer_typedef x = null\n"
                                                  "end\n");

    EXPECT_EQ("#include \"/tmp/ThreeTestData/CSources/Typedefs.h\"\n\n"
              "void test(void);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(void) {\n"
              "    const pointer_typedef x = NULL;\n"
              "}\n\n", visitor->bodyString());
}
