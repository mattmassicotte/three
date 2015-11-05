#include "../ParserTestsBase.h"

class ParserTests_Types : public ParserTestsBase {
};

TEST_F(ParserTests_Types, GlobalBoolean) {
    ASTNode* node = this->parseNode("Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    ASSERT_EQ(DataType::Boolean, node->dataType().kind());
    ASSERT_EQ(DataType::Access::Read, node->dataType().access());
}

TEST_F(ParserTests_Types, GlobalUntyped) {
    ASTNode* node = this->parseNode("value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    ASSERT_EQ(DataType::Undefined, node->dataType().kind());
}

TEST_F(ParserTests_Types, GlobalInt) {
    ASTNode* node = this->parseNode("Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithWidthSpecifier) {
    ASTNode* node = this->parseNode("Int:32 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(32, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithAlignmentSpecifier) {
    ASTNode* node = this->parseNode("Int::4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int:::4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithWidthAndAlignmentSpecifier) {
    ASTNode* node = this->parseNode("Int:32:4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(32, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithWidthAndVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int:32::4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(32, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithAlignmentAndVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int::4:4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalIntWithWidthAlignmentAndVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int:16:4:4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Integer, node->dataType().kind());
    ASSERT_EQ(16, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalNatural) {
    ASTNode* node = this->parseNode("Natural value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Natural, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalNaturalWithWidth) {
    ASTNode* node = this->parseNode("Natural:8 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Natural, node->dataType().kind());
    ASSERT_EQ(8, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalFloat) {
    ASTNode* node = this->parseNode("Float value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Float, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalReal) {
    ASTNode* node = this->parseNode("Real value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Real, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(ParserTests_Types, GlobalChar) {
    ASTNode* node = this->parseNode("Char value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF8, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithAscii) {
    ASTNode* node = this->parseNode("Char:ascii value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::ASCII, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF8) {
    ASTNode* node = this->parseNode("Char:utf8 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF8, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF16) {
    ASTNode* node = this->parseNode("Char:utf16 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF16, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF16LE) {
    ASTNode* node = this->parseNode("Char:utf16le value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF16LE, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF16BE) {
    ASTNode* node = this->parseNode("Char:utf16be value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF16BE, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF32) {
    ASTNode* node = this->parseNode("Char:utf32 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF32, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF32LE) {
    ASTNode* node = this->parseNode("Char:utf32le value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF32LE, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalCharWithUTF32BE) {
    ASTNode* node = this->parseNode("Char:utf32be value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Character, node->dataType().kind());
    ASSERT_EQ(DataType::CharacterEncoding::UTF32BE, node->dataType().characterEncoding());
}

TEST_F(ParserTests_Types, GlobalPointerToInt) {
    ASTNode* node = this->parseNode("*Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, GlobalPointerToPointerToInt) {
    ASTNode* node = this->parseNode("**Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Pointer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, GlobalArrayOfInt) {
    ASTNode* node = this->parseNode("[3]Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Array, node->dataType().kind());
    ASSERT_EQ(3, node->dataType().arrayCount());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(0, node->dataType().subtypeAtIndex(0).subtypeCount());
}

TEST_F(ParserTests_Types, GlobalArrayOfPointersToInt) {
    ASTNode* node = this->parseNode("[3]*Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Array, node->dataType().kind());
    ASSERT_EQ(3, node->dataType().arrayCount());
    ASSERT_EQ(DataType::Pointer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, GlobalPointerToArrayOfInt) {
    ASTNode* node = this->parseNode("*[5]Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Array, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(5, node->dataType().subtypeAtIndex(0).arrayCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, TupleOfSingleInt) {
    ASTNode* node = this->parseNode("(Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Tuple, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, TupleOfTwoInts) {
    ASTNode* node = this->parseNode("(Int, Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Tuple, node->dataType().kind());
    ASSERT_EQ(2, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Integer, node->dataType().subtypeAtIndex(1).kind());
}

TEST_F(ParserTests_Types, GlobalFunction) {
    ASTNode* node = this->parseNode("() -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("value", node->name());
    ASSERT_EQ(DataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalFunctionTakingSingleParam) {
    ASTNode* node = this->parseNode("(Int) -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Integer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalFunctionTakingTwoParams) {
    ASTNode* node = this->parseNode("(Int, Float) -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(2, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Integer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Float, node->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalFunctionTakingAPointerParam) {
    ASTNode* node = this->parseNode("(*Int) -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Pointer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalFunctionWithReturn) {
    ASTNode* node = this->parseNode("() -> Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(1, node->dataType().returnCount());
    ASSERT_EQ(DataType::Integer, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalFunctionWithTwoReturns) {
    ASTNode* node = this->parseNode("() -> (Int, Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());

    ASSERT_EQ(DataType::Tuple, node->dataType().returnType().kind());
    ASSERT_EQ(2, node->dataType().returnType().subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().returnType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Integer, node->dataType().returnType().subtypeAtIndex(1).kind());
}

TEST_F(ParserTests_Types, GlobalPointerToFunction) {
    ASTNode* node = this->parseNode("*(Int) -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Function, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, GlobalPointerToFunctionWithPointerArg) {
    ASTNode* node = this->parseNode("*(*Void) -> Void value\n");

    ASSERT_EQ(1, node->childCount());
    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Function, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, GlobalClosure) {
    ASTNode* node = this->parseNode("{} -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("value", node->name());
    ASSERT_EQ(DataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Kind::Pointer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().parameterAtIndex(0).subtypeCount());
    ASSERT_EQ(DataType::Kind::Void, node->dataType().parameterAtIndex(0).subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalClosureTakingSingleParam) {
    ASTNode* node = this->parseNode("{Int} -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(2, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Integer, node->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalClosureTakingTwoParams) {
    ASTNode* node = this->parseNode("{Int, Float} -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(3, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Integer, node->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Float, node->dataType().parameterAtIndex(2).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalClosureTakingAPointerParam) {
    ASTNode* node = this->parseNode("{*Int} -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(2, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Pointer, node->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(DataType::Void, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalClosureWithReturn) {
    ASTNode* node = this->parseNode("{} -> Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(1, node->dataType().returnCount());
    ASSERT_EQ(DataType::Integer, node->dataType().returnType().kind());
}

TEST_F(ParserTests_Types, GlobalClosureWithTwoReturns) {
    ASTNode* node = this->parseNode("{} -> (Int, Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(DataType::Tuple, node->dataType().returnType().kind());
    ASSERT_EQ(2, node->dataType().returnType().subtypeCount());
    ASSERT_EQ(DataType::Integer, node->dataType().returnType().subtypeAtIndex(0).kind());
    ASSERT_EQ(DataType::Integer, node->dataType().returnType().subtypeAtIndex(1).kind());
}

TEST_F(ParserTests_Types, GlobalPointerToClosure) {
    ASTNode* node = this->parseNode("*{Int} -> Void value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(DataType::Closure, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Types, VarargVariable) {
    ASTNode* node = this->parseNode("Vararg value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(DataType::Vararg, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
}

TEST_F(ParserTests_Types, GlobalMutableBoolean) {
    ASTNode* node = this->parseNode("Bool! value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    EXPECT_EQ(DataType::Boolean, node->dataType().kind());
    EXPECT_EQ(DataType::Access::ReadWrite, node->dataType().access());
}

TEST_F(ParserTests_Types, GlobalOptionalBooleanPointer) {
    ASTNode* node = this->parseNode("Bool? value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    EXPECT_EQ(DataType::NullablePointer, node->dataType().kind());
    EXPECT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    EXPECT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::Read, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Types, GlobalOptionalMutableBooleanPointer) {
    ASTNode* node = this->parseNode("Bool!? value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    EXPECT_EQ(DataType::NullablePointer, node->dataType().kind());
    EXPECT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    EXPECT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::ReadWrite, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Types, GlobalMutableBooleanPointer) {
    ASTNode* node = this->parseNode("*Bool! value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    EXPECT_EQ(DataType::Pointer, node->dataType().kind());
    EXPECT_EQ(DataType::Access::Read, node->dataType().access());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    EXPECT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::ReadWrite, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Types, GlobalMutableBooleanMutablePointer) {
    ASTNode* node = this->parseNode("*!Bool! value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    EXPECT_EQ(DataType::Pointer, node->dataType().kind());
    EXPECT_EQ(DataType::Access::ReadWrite, node->dataType().access());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    EXPECT_EQ(DataType::Boolean, node->dataType().subtypeAtIndex(0).kind());
    EXPECT_EQ(DataType::Access::ReadWrite, node->dataType().subtypeAtIndex(0).access());
}

TEST_F(ParserTests_Types, GlobalMutableBooleanPointerToMutablePointer) {
    ASTNode* node = this->parseNode("*!*Bool! value\n");

    node = node->childAtIndex(0);

    DataType type = node->dataType();
    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    EXPECT_EQ(DataType::Pointer, type.kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.access());
    ASSERT_EQ(1, node->dataType().subtypeCount());

    type = type.subtypeAtIndex(0);
    EXPECT_EQ(DataType::Pointer, type.kind());
    EXPECT_EQ(DataType::Access::Read, type.access());
    ASSERT_EQ(1, type.subtypeCount());

    type = type.subtypeAtIndex(0);
    EXPECT_EQ(DataType::Boolean, type.kind());
    EXPECT_EQ(DataType::Access::ReadWrite, type.access());
}
