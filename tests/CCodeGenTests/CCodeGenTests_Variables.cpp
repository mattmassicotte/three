#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Variables : public ParserTestBase {
};


TEST_F(CCodeGenTests_Variables, VariableDeclarationWithInitializer) {
    ASTNode* node = this->parse("def test(Int a)\nInt b = a\nend\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("void test(int a);\n", visitor.internalHeaderString());
    EXPECT_EQ("void test(int a) {\n    int b = a;\n}\n\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Variables, SimpleGlobalVariable) {
    ASTNode* node = this->parse("Int a\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("extern int a;\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("int a;\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Variables, FunctionPointerVariable) {
    ASTNode* node = this->parse("*(*Void; *Void) fn_ptr\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("extern void* (*fn_ptr)(void*);\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("void* (*fn_ptr)(void*);\n", visitor.bodyString());
}
