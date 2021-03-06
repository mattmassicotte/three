#include "CCodeGenTestsBase.h"

class CCodeGenTests_Variables : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Variables, VariableDeclarationWithInitializer) {
    Three::CCodeGenVisitor* visitor = this->visit("def test(Int a)\nInt b = a\nend\n");

    EXPECT_EQ("void test(const int a);\n", visitor->internalHeaderString());
    EXPECT_EQ("void test(const int a) {\n    const int b = a;\n}\n\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Variables, SimpleGlobalVariable) {
    Three::CCodeGenVisitor* visitor = this->visit("Int a = 0\n");

    EXPECT_EQ("extern const int a;\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("const int a = 0;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Variables, PrivateGlobalVariable) {
    Three::CCodeGenVisitor* visitor = this->visit("private\nInt a\n");

    EXPECT_EQ("", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("static const int a;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Variables, PublicGlobalVariable) {
    Three::CCodeGenVisitor* visitor = this->visit("public\nInt a = 0\n");

    EXPECT_EQ("", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\nextern const int a;\n", visitor->externalHeaderString());
    EXPECT_EQ("const int a = 0;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Variables, FunctionPointerVariable) {
    Three::CCodeGenVisitor* visitor = this->visit("*(*Void) -> *Void fn_ptr\n");

    EXPECT_EQ("extern void* const (* const fn_ptr)(void* const);\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("void* const (* const fn_ptr)(void* const);\n", visitor->bodyString());
}
