#include "compiler/Lexer/Lexer.h"
#include "compiler/Parser/ParseHelper.h"
#include "compiler/Parser/TypeParser.h"

#include <assert.h>
#include <gtest/gtest.h>

class TypeParserTests : public testing::Test {
protected:
    virtual void SetUp() {
        _stream = nullptr;
        _lexer  = nullptr;
        _helper = nullptr;
        _context = nullptr;
        _typeParser = nullptr;
    }

    virtual void TearDown() {
        delete _typeParser;
        delete _context;
        delete _helper;
        delete _lexer;
        delete _stream;
    }

    void setInput(const char* input) {
        std::string inputString(input);

        assert(!_stream);
        _stream = new std::istringstream(inputString);

        assert(!_lexer);
        _lexer = new Three::Lexer(_stream);

        assert(!_helper);
        _helper = new Three::ParseHelper(_lexer);

        assert(!_context);
        _context = new Three::ParseContext();

        assert(!_typeParser);
        _typeParser = new Three::TypeParser(_helper, _context);
    }

    Three::TypeParser* typeParser() const {
        return _typeParser;
    }

    Three::Token peek() {
        return _helper->peek();
    }

private:
    std::istringstream* _stream;
    Three::Lexer* _lexer;
    Three::ParseHelper* _helper;
    Three::ParseContext* _context;
    Three::TypeParser* _typeParser;
};

TEST_F(TypeParserTests, TestPeekIntType) {
    this->setInput("Int a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekIntWithWidthType) {
    this->setInput("Int:32 a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekIntWithWidthAlignAndVectorType) {
    this->setInput("Int:32:4:4 a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekPointerIntType) {
    this->setInput("*Int a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekOneTupleType) {
    this->setInput("(Int) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekTwoTupleType) {
    this->setInput("(Int, Int) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekThreeTupleType) {
    this->setInput("(Int, Int, Int) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekTwoTupleWithNamedParamsType) {
    this->setInput("(Int x, Int y) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekFunctionType) {
    this->setInput("() -> () a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekFunctionReturnsPlainType) {
    this->setInput("() -> Int a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekFunctionReturnsTupleType) {
    this->setInput("() -> (Int) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekFunctionReturnsTwoTupleType) {
    this->setInput("() -> (Int, Int) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekClosureReturnsTwoTupleType) {
    this->setInput("{} -> (Int, Int) a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekOptionalIntType) {
    this->setInput("Int? a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestPeekMutableBooleanType) {
    this->setInput("Bool! a");

    ASSERT_TRUE(this->typeParser()->isAtType());
}

TEST_F(TypeParserTests, TestIntDataTypeParsing) {
    this->setInput("Int a");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Integer, type.kind());
    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestIntWithSizeDataTypeParsing) {
    this->setInput("Int:32 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Integer, type.kind());
    ASSERT_EQ(32, type.widthSpecifier());
    ASSERT_EQ(0, type.alignmentSpecifier());
    ASSERT_EQ(0, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestIntWithAlignmentDataTypeParsing) {
    this->setInput("Int::4 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Integer, type.kind());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(4, type.alignmentSpecifier());
    ASSERT_EQ(0, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestIntWithVectorDataTypeParsing) {
    this->setInput("Int:::4 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Integer, type.kind());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(0, type.alignmentSpecifier());
    ASSERT_EQ(4, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestIntWithSizeAlignmentAndVectorDataTypeParsing) {
    this->setInput("Int:16:4:4 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Integer, type.kind());
    ASSERT_EQ(16, type.widthSpecifier());
    ASSERT_EQ(4, type.alignmentSpecifier());
    ASSERT_EQ(4, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestNaturalDataTypeParsing) {
    this->setInput("Natural a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Natural, type.kind());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(0, type.alignmentSpecifier());
    ASSERT_EQ(0, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestNaturalWithSizeDataTypeParsing) {
    this->setInput("Natural:8 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Natural, type.kind());
    ASSERT_EQ(8, type.widthSpecifier());
    ASSERT_EQ(0, type.alignmentSpecifier());
    ASSERT_EQ(0, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestFloatDataTypeParsing) {
    this->setInput("Float a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Float, type.kind());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(0, type.alignmentSpecifier());
    ASSERT_EQ(0, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestRealDataTypeParsing) {
    this->setInput("Real a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Real, type.kind());
    ASSERT_EQ(0, type.widthSpecifier());
    ASSERT_EQ(0, type.alignmentSpecifier());
    ASSERT_EQ(0, type.vectorSizeSpecifier());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestCharTypeParsing) {
    this->setInput("Char a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF8, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharASCIITypeParsing) {
    this->setInput("Char:ascii a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::ASCII, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF8TypeParsing) {
    this->setInput("Char:utf8 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF8, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF16TypeParsing) {
    this->setInput("Char:utf16 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF16, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF16LETypeParsing) {
    this->setInput("Char:utf16le a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF16LE, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF16BETypeParsing) {
    this->setInput("Char:utf16be a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF16BE, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF32TypeParsing) {
    this->setInput("Char:utf32 a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF32, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF32LETypeParsing) {
    this->setInput("Char:utf32le a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF32LE, type.characterEncoding());
}

TEST_F(TypeParserTests, TestCharUTF32BETypeParsing) {
    this->setInput("Char:utf32be a");

    DataType type = this->typeParser()->parseType();
    ASSERT_EQ(DataType::Character, type.kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF32BE, type.characterEncoding());
}

TEST_F(TypeParserTests, TestIntPointerDataTypeParsing) {
    this->setInput("*Int a");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());

    type = type.subtypeAtIndex(0);
    ASSERT_EQ(DataType::Integer, type.kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestIntPointerToPointerDataTypeParsing) {
    this->setInput("**Int a");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());

    type = type.subtypeAtIndex(0);
    ASSERT_EQ(DataType::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());

    type = type.subtypeAtIndex(0);
    ASSERT_EQ(DataType::Integer, type.kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestIntArrayDataTypeParsing) {
    this->setInput("[]Int a");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Array, type.kind());
    ASSERT_EQ(0, type.arrayCount());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).kind());
    ASSERT_EQ(0, type.subtypeAtIndex(0).subtypeCount());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TestSizedIntArrayDataTypeParsing) {
    this->setInput("[3]Int a");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Array, type.kind());
    ASSERT_EQ(3, type.arrayCount());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).kind());
    ASSERT_EQ(0, type.subtypeAtIndex(0).subtypeCount());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, ArrayOfPointersToIntParsing) {
    this->setInput("[3]*Int a\n");

    DataType type = this->typeParser()->parseType();
    
    ASSERT_EQ(DataType::Array, type.kind());
    ASSERT_EQ(3, type.arrayCount());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Pointer, type.subtypeAtIndex(0).kind());
    ASSERT_EQ(1, type.subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).subtypeAtIndex(0).kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, PointerToArrayOfIntParsing) {
    this->setInput("*[5]Int a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Array, type.subtypeAtIndex(0).kind());
    ASSERT_EQ(1, type.subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(5, type.subtypeAtIndex(0).arrayCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).subtypeAtIndex(0).kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TupleOfSingleInt) {
    this->setInput("(Int) a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Tuple, type.kind());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TupleOfTwoInts) {
    this->setInput("(Int, Int) a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Tuple, type.kind());
    ASSERT_EQ(2, type.subtypeCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(1).kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, TupleOfTwoNamedInts) {
    this->setInput("(Int x, Int y) a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Tuple, type.kind());
    ASSERT_EQ(2, type.subtypeCount());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(0).kind());
    ASSERT_EQ("x", type.subtypeAtIndex(0).label());
    ASSERT_EQ(DataType::Integer, type.subtypeAtIndex(1).kind());
    ASSERT_EQ("y", type.subtypeAtIndex(1).label());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, Function) {
    this->setInput("() -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, FunctionWithNonVoidReturn) {
    this->setInput("() -> Int a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Integer, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, FunctionWithTupleReturn) {
    this->setInput("() -> (Int, Int) a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Tuple, type.returnType().kind());
    ASSERT_EQ(2, type.returnType().subtypeCount());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, FunctionWithOneParam) {
    this->setInput("(Int) -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(1, type.parameterCount());
    ASSERT_EQ(DataType::Integer, type.parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, FunctionWithTwoParams) {
    this->setInput("(Int, Float) -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(2, type.parameterCount());
    ASSERT_EQ(DataType::Integer, type.parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Float, type.parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, FunctionWithThreeParams) {
    this->setInput("(Int, Float, Int) -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(3, type.parameterCount());
    ASSERT_EQ(DataType::Integer, type.parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Float, type.parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Integer, type.parameterAtIndex(2).kind());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, FunctionWithOnePointerParam) {
    this->setInput("(*Int) -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Function, type.kind());
    ASSERT_EQ(1, type.parameterCount());
    ASSERT_EQ(DataType::Pointer, type.parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Integer, type.parameterAtIndex(0).subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, PointerToFunction) {
    this->setInput("*() -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Function, type.subtypeAtIndex(0).kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, PointerToFunctionWithOnePointerParam) {
    this->setInput("*(*Int) -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Pointer, type.kind());
    ASSERT_EQ(1, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Function, type.subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Pointer, type.subtypeAtIndex(0).parameterAtIndex(0).kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, Closure) {
    this->setInput("{} -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Closure, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(1, type.parameterCount());
    ASSERT_EQ(DataType::Kind::Pointer, type.parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Void, type.parameterAtIndex(0).subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, ClosureWithNonVoidReturn) {
    this->setInput("{} -> Int a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Closure, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Integer, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, ClosureWithTupleReturn) {
    this->setInput("{} -> (Int, Int) a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Closure, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(DataType::Kind::Tuple, type.returnType().kind());
    ASSERT_EQ(2, type.returnType().subtypeCount());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, ClosureWithOneParam) {
    this->setInput("{Int} -> Void a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Closure, type.kind());
    ASSERT_EQ(0, type.subtypeCount());
    ASSERT_EQ(2, type.parameterCount());
    ASSERT_EQ(DataType::Integer, type.parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Kind::Void, type.returnType().kind());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, Vararg) {
    this->setInput("Vararg a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Vararg, type.kind());
    ASSERT_EQ(0, type.subtypeCount());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, MutableBoolean) {
    this->setInput("Bool! a\n");

    DataType type = this->typeParser()->parseType();

    ASSERT_EQ(DataType::Kind::Boolean, type.kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.access());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, OptionalBoolean) {
    this->setInput("Bool? a\n");

    DataType type = this->typeParser()->parseType();

    EXPECT_EQ(DataType::NullablePointer, type.kind());
    EXPECT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(1, type.subtypeCount());
    EXPECT_EQ(DataType::Boolean, type.subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::Read, type.subtypeAtIndex(0).access());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, OptionalMutableBoolean) {
    this->setInput("Bool!? a\n");

    DataType type = this->typeParser()->parseType();

    EXPECT_EQ(DataType::NullablePointer, type.kind());
    EXPECT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(1, type.subtypeCount());
    EXPECT_EQ(DataType::Boolean, type.subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.subtypeAtIndex(0).access());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, MutablePointerToMutableBoolean) {
    this->setInput("*!Bool! a\n");

    DataType type = this->typeParser()->parseType();

    EXPECT_EQ(DataType::Pointer, type.kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.access());
    ASSERT_EQ(1, type.subtypeCount());
    EXPECT_EQ(DataType::Boolean, type.subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.subtypeAtIndex(0).access());

    ASSERT_EQ("a", this->peek().str());
}

TEST_F(TypeParserTests, MutablePointerToPointerToMutableBoolean) {
    this->setInput("*!*Bool! a\n");

    DataType type = this->typeParser()->parseType();

    EXPECT_EQ(DataType::Pointer, type.kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.access());
    ASSERT_EQ(1, type.subtypeCount());

    type = type.subtypeAtIndex(0);
    EXPECT_EQ(DataType::Pointer, type.kind());
    EXPECT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(1, type.subtypeCount());

    type = type.subtypeAtIndex(0);
    EXPECT_EQ(DataType::Boolean, type.kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.access());

    ASSERT_EQ("a", this->peek().str());
}
