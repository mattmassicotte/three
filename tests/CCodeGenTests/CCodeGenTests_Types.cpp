#include "CCodeGenTestsBase.h"

class CCodeGenTests_Types : public CCodeGenTestsBase {
};

TEST_F(CCodeGenTests_Types, Structure) {
    Three::CCodeGenVisitor* visitor = this->visit("struct Foo\n"
                                                  "  Int x\n"
                                                  "  Int y\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n"
              "    int x;\n"
              "    int y;\n"
              "} Foo;\n\n", visitor->internalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, PublicStructure) {
    Three::CCodeGenVisitor* visitor = this->visit("public\n"
                                                  "struct Foo\n"
                                                  "  Int x\n"
                                                  "  Int y\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n"
              "    int x;\n"
              "    int y;\n"
              "} Foo;\n\n", visitor->internalHeaderString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n"
              "typedef struct Foo Foo;\n\n", visitor->externalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, PrivateStructure) {
    Three::CCodeGenVisitor* visitor = this->visit("private\n"
                                                  "struct Foo\n"
                                                  "  Int x\n"
                                                  "end\n");

    EXPECT_EQ("typedef struct Foo {\n    int x;\n} Foo;\n\n", visitor->internalHeaderString());
    EXPECT_EQ("", visitor->declarationsString());
    EXPECT_EQ("#include <three/runtime/types.h>\n\n", visitor->externalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, PackedStructure) {
    Three::CCodeGenVisitor* visitor = this->visit("struct:4 Foo\n"
                                                  "  Int x\n"
                                                  "  Int y\n"
                                                  "end\n");

    EXPECT_EQ("#pragma pack(push)\n"
              "#pragma pack(4)\n"
              "typedef struct Foo {\n"
              "    int x;\n"
              "    int y;\n"
              "} Foo;\n"
              "#pragma pack(pop)\n\n", visitor->internalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, Enumeration) {
    Three::CCodeGenVisitor* visitor = this->visit("enum Bar\n"
                                                  "  A\n"
                                                  "  B\n"
                                                  "end\n");

    EXPECT_EQ("enum {\n"
              "    Bar_3_A,\n"
              "    Bar_3_B\n"
              "};\n"
              "typedef uint32_t Bar;\n\n", visitor->internalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, DISABLED_NamespacedEnumeration) {
    Three::CCodeGenVisitor* visitor = this->visit("namespace Foo\n"
                                                  "  enum Bar\n"
                                                  "    A\n"
                                                  "    B\n"
                                                  "  end\n"
                                                  "end\n");

    EXPECT_EQ("enum {\n"
              "    Foo_3_Bar_3_A,\n"
              "    Foo_3_Bar_3_B\n"
              "};\n"
              "typedef uint32_t Foo_3_Bar;\n\n", visitor->internalHeaderString());
    EXPECT_EQ("", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleBooleans) {
    Three::CCodeGenVisitor* visitor = this->visit("Bool value\n");

    EXPECT_EQ("const bool value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleMutableBooleans) {
    Three::CCodeGenVisitor* visitor = this->visit("Bool! value\n");

    EXPECT_EQ("bool value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleInteger) {
    Three::CCodeGenVisitor* visitor = this->visit("Int value\n");

    EXPECT_EQ("const int value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleNatural) {
    Three::CCodeGenVisitor* visitor = this->visit("Natural value\n");

    EXPECT_EQ("const unsigned int value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleNatural32) {
    Three::CCodeGenVisitor* visitor = this->visit("Natural:32 value\n");

    EXPECT_EQ("const uint32_t value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleFloat) {
    Three::CCodeGenVisitor* visitor = this->visit("Float value\n");

    EXPECT_EQ("const float value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, SimpleFloat64) {
    Three::CCodeGenVisitor* visitor = this->visit("Float:64 value\n");

    EXPECT_EQ("const double value;\n", visitor->bodyString());
}

TEST_F(CCodeGenTests_Types, PointerToInteger) {
    Three::CCodeGenVisitor* visitor = this->visit("*Int value\n");

    EXPECT_EQ("const int* const value;\n", visitor->bodyString());
}
