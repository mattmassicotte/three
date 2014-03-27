#include "../ParserTestBase.h"
#include "compiler/Operations/CCodeGenVisitor.h"

class CCodeGenTests_Types : public ParserTestBase {
};

TEST_F(CCodeGenTests_Types, Structure) {
    ASTNode* node = this->parse("struct Foo\n"
                                "  Int x\n"
                                "  Int y\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("typedef struct Foo {\n    int x;\n    int y;\n} Foo;\n\n", visitor.internalHeaderString());
    EXPECT_EQ("", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, PublicStructure) {
    ASTNode* node = this->parse("public\n"
                                "struct Foo\n"
                                "  Int x\n"
                                "  Int y\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("typedef struct Foo {\n    int x;\n    int y;\n} Foo;\n\n", visitor.internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n"
              "typedef struct Foo Foo;\n\n", visitor.externalHeaderString());
    EXPECT_EQ("", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, PrivateStructure) {
    ASTNode* node = this->parse("private\n"
                                "struct Foo\n"
                                "  Int x\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("typedef struct Foo {\n    int x;\n} Foo;\n\n", visitor.internalHeaderString());
    EXPECT_EQ("", visitor.declarationsString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor.externalHeaderString());
    EXPECT_EQ("", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, PackedStructure) {
    ASTNode* node = this->parse("struct:4 Foo\n"
                                "  Int x\n"
                                "  Int y\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("#pragma pack(push)\n"
              "#pragma pack(4)\n"
              "typedef struct Foo {\n"
              "    int x;\n"
              "    int y;\n"
              "} Foo;\n"
              "#pragma pack(pop)\n\n", visitor.internalHeaderString());
    EXPECT_EQ("", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, NamespacedEnumeration) {
    ASTNode* node = this->parse("namespace Foo\n"
                                "  enum Bar\n"
                                "    A\n"
                                "    B\n"
                                "  end\n"
                                "end\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("enum {\n"
              "    Foo_3_Bar_3_A,\n"
              "    Foo_3_Bar_3_B\n"
              "};\n"
              "typedef uint32_t Foo_3_Bar;\n\n", visitor.internalHeaderString());
    EXPECT_EQ("", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleBooleans) {
    ASTNode* node = this->parse("Bool value\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("bool value;\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleInteger) {
    ASTNode* node = this->parse("Int value\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("int value;\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleNatural) {
    ASTNode* node = this->parse("Natural value\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("unsigned int value;\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleFloat) {
    ASTNode* node = this->parse("Float value\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("float value;\n", visitor.bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleFloat64) {
    ASTNode* node = this->parse("Float:64 value\n");

    CCodeGenVisitor visitor;

    node->accept(visitor);

    EXPECT_EQ("double value;\n", visitor.bodyString());
}
