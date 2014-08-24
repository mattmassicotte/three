#include "compiler/CodeGen/CTypeCodeGenerator.h"

class CTypeCodeGeneratorTests : public testing::Test {
};

TEST_F(CTypeCodeGeneratorTests, Int) {
    NewDataType type(NewDataType::Kind::Integer);
    type.setAccess(NewDataType::Access::ReadWrite);

    EXPECT_EQ("int", CTypeCodeGenerator::codeGen(type));
}

TEST_F(CTypeCodeGeneratorTests, MutableInt) {
    NewDataType type(NewDataType::Kind::Integer);

    EXPECT_EQ("const int", CTypeCodeGenerator::codeGen(type));
}

TEST_F(CTypeCodeGeneratorTests, MutablePointerToMutableInt) {
    NewDataType type(NewDataType::Kind::Integer);
    type.setAccess(NewDataType::Access::ReadWrite);

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::ReadWrite);

    EXPECT_EQ("int*", CTypeCodeGenerator::codeGen(type));
}

TEST_F(CTypeCodeGeneratorTests, PointerToInt) {
    NewDataType type(NewDataType::Kind::Integer);
    type.setAccess(NewDataType::Access::Read);

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::Read);

    EXPECT_EQ("const int* const", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const int* const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, MutablePointerToPointerInt) {
    NewDataType type(NewDataType::Kind::Integer);
    type.setAccess(NewDataType::Access::Read);

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::Read);

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::ReadWrite);

    EXPECT_EQ("const int* const*", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const int* const* x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, FunctionPointer) {
    NewDataType type(NewDataType::Kind::Function);
    type.addSubtype(NewDataType(NewDataType::Kind::Void));
    type.addReturn(NewDataType(NewDataType::Kind::Void));
    type.setAccess(NewDataType::Access::Read);

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::Read);

    EXPECT_EQ("void (* const)(void)", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("void (* const x)(void)", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, Closure) {
    NewDataType type(NewDataType::Kind::Closure);
    type.addReturn(NewDataType(NewDataType::Kind::Void));
    type.setAccess(NewDataType::Access::Read);

    // EXPECT_EQ("void (* const)(void)", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const three_closure_t x", CTypeCodeGenerator::codeGen(type, "x"));

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::Read);

    // EXPECT_EQ("void (* const)(void)", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const three_closure_t* const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, Structure) {
    NewDataType type(NewDataType::Kind::Structure);
    type.addSubtype(NewDataType(NewDataType::Kind::Integer));
    type.setAccess(NewDataType::Access::Read);
    type.setName("Foo");

    EXPECT_EQ("const Foo", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const Foo x", CTypeCodeGenerator::codeGen(type, "x"));

    type = NewDataType::wrapInPointer(type);
    type.setAccess(NewDataType::Access::Read);

    EXPECT_EQ("const Foo* const", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const Foo* const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, NullablePointer) {
    NewDataType type(NewDataType::Kind::NullablePointer);
    type.addSubtype(NewDataType(NewDataType::Kind::Integer));
    type.setAccess(NewDataType::Access::Read);

    EXPECT_EQ("const int* THREE_ATTR_NULLABLE const", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const int* THREE_ATTR_NULLABLE const x", CTypeCodeGenerator::codeGen(type, "x"));
}
