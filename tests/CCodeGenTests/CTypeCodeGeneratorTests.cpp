#include "compiler/CodeGen/CTypeCodeGenerator.h"

class CTypeCodeGeneratorTests : public testing::Test {
};

TEST_F(CTypeCodeGeneratorTests, Int) {
    DataType type(DataType::Kind::Integer);
    type.setAccess(DataType::Access::ReadWrite);

    EXPECT_EQ("int", CTypeCodeGenerator::codeGen(type));
}

TEST_F(CTypeCodeGeneratorTests, MutableInt) {
    DataType type(DataType::Kind::Integer);

    EXPECT_EQ("const int", CTypeCodeGenerator::codeGen(type));
}

TEST_F(CTypeCodeGeneratorTests, MutablePointerToMutableInt) {
    DataType type(DataType::Kind::Integer);
    type.setAccess(DataType::Access::ReadWrite);

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::ReadWrite);

    EXPECT_EQ("int*", CTypeCodeGenerator::codeGen(type));
}

TEST_F(CTypeCodeGeneratorTests, PointerToInt) {
    DataType type(DataType::Kind::Integer);
    type.setAccess(DataType::Access::Read);

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::Read);

    EXPECT_EQ("const int* const", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const int* const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, MutablePointerToPointerInt) {
    DataType type(DataType::Kind::Integer);
    type.setAccess(DataType::Access::Read);

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::Read);

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::ReadWrite);

    EXPECT_EQ("const int* const*", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const int* const* x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, FunctionPointer) {
    DataType type(DataType::Kind::Function);
    type.addSubtype(DataType(DataType::Kind::Void));
    type.addReturn(DataType(DataType::Kind::Void));
    type.setAccess(DataType::Access::Read);

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::Read);

    EXPECT_EQ("void (* const)(void)", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("void (* const x)(void)", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, Closure) {
    DataType type(DataType::Kind::Closure);
    type.addReturn(DataType(DataType::Kind::Void));
    type.setAccess(DataType::Access::Read);

    EXPECT_EQ("const three_closure_t x", CTypeCodeGenerator::codeGen(type, "x"));

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::Read);

    EXPECT_EQ("const three_closure_t* const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, Structure) {
    DataType type(DataType::Kind::Structure);
    type.addSubtype(DataType(DataType::Kind::Integer));
    type.setAccess(DataType::Access::Read);
    type.setName(QualifiedName("Foo"));

    EXPECT_EQ("const Foo", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const Foo x", CTypeCodeGenerator::codeGen(type, "x"));

    type = DataType::wrapInPointer(type);
    type.setAccess(DataType::Access::Read);

    EXPECT_EQ("const Foo* const", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const Foo* const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, NullablePointer) {
    DataType type(DataType::Kind::NullablePointer);
    type.addSubtype(DataType(DataType::Kind::Integer));
    type.setAccess(DataType::Access::Read);

    EXPECT_EQ("const int* THREE_ATTR_NULLABLE const", CTypeCodeGenerator::codeGen(type));
    EXPECT_EQ("const int* THREE_ATTR_NULLABLE const x", CTypeCodeGenerator::codeGen(type, "x"));
}

TEST_F(CTypeCodeGeneratorTests, ConstIntArray) {
    DataType type(DataType::Kind::Array);
    type.setAccess(DataType::Access::ReadWrite);
    type.setArrayCount(5);
    type.addSubtype(DataType(DataType::Kind::Integer));

    EXPECT_EQ("const int[5]", CTypeCodeGenerator::codeGen(type));
}
