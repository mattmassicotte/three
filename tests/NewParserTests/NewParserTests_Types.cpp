#include "../ParserTestsBase.h"

class NewParserTests_Types : public ParserTestsBase {
};

TEST_F(NewParserTests_Types, GlobalBoolean) {
    ASTNode* node = this->parseNode("Bool value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    ASSERT_EQ(NewDataType::Boolean, node->dataType().kind());
}

TEST_F(NewParserTests_Types, GlobalUntyped) {
    ASTNode* node = this->parseNode("value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Variable Declaration", node->nodeName());
    ASSERT_EQ("value", node->name());
    ASSERT_EQ(NewDataType::Undefined, node->dataType().kind());
}

TEST_F(NewParserTests_Types, GlobalInt) {
    ASTNode* node = this->parseNode("Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithWidthSpecifier) {
    ASTNode* node = this->parseNode("Int:32 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(32, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithAlignmentSpecifier) {
    ASTNode* node = this->parseNode("Int::4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int:::4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithWidthAndAlignmentSpecifier) {
    ASTNode* node = this->parseNode("Int:32:4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(32, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithWidthAndVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int:32::4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(32, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithAlignmentAndVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int::4:4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalIntWithWidthAlignmentAndVectorSizeSpecifier) {
    ASTNode* node = this->parseNode("Int:16:4:4 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Integer, node->dataType().kind());
    ASSERT_EQ(16, node->dataType().widthSpecifier());
    ASSERT_EQ(4, node->dataType().alignmentSpecifier());
    ASSERT_EQ(4, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalNatural) {
    ASTNode* node = this->parseNode("Natural value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Natural, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalNaturalWithWidth) {
    ASTNode* node = this->parseNode("Natural:8 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Natural, node->dataType().kind());
    ASSERT_EQ(8, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalFloat) {
    ASTNode* node = this->parseNode("Float value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Float, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalReal) {
    ASTNode* node = this->parseNode("Real value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Real, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().widthSpecifier());
    ASSERT_EQ(0, node->dataType().alignmentSpecifier());
    ASSERT_EQ(0, node->dataType().vectorSizeSpecifier());
}

TEST_F(NewParserTests_Types, GlobalChar) {
    ASTNode* node = this->parseNode("Char value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF8, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithAscii) {
    ASTNode* node = this->parseNode("Char:ascii value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::ASCII, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF8) {
    ASTNode* node = this->parseNode("Char:utf8 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF8, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF16) {
    ASTNode* node = this->parseNode("Char:utf16 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF16, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF16LE) {
    ASTNode* node = this->parseNode("Char:utf16le value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF16LE, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF16BE) {
    ASTNode* node = this->parseNode("Char:utf16be value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF16BE, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF32) {
    ASTNode* node = this->parseNode("Char:utf32 value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF32, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF32LE) {
    ASTNode* node = this->parseNode("Char:utf32le value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF32LE, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalCharWithUTF32BE) {
    ASTNode* node = this->parseNode("Char:utf32be value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Character, node->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF32BE, node->dataType().characterEncoding());
}

TEST_F(NewParserTests_Types, GlobalPointerToInt) {
    ASTNode* node = this->parseNode("*Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalPointerToPointerToInt) {
    ASTNode* node = this->parseNode("**Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Pointer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().subtypeAtIndex(0).subtypeAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalArrayOfInt) {
    ASTNode* node = this->parseNode("[3]Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Array, node->dataType().kind());
    ASSERT_EQ(3, node->dataType().arrayCount());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(0, node->dataType().subtypeAtIndex(0).subtypeCount());
}

TEST_F(NewParserTests_Types, GlobalArrayOfPointersToInt) {
    ASTNode* node = this->parseNode("[3]*Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Array, node->dataType().kind());
    ASSERT_EQ(3, node->dataType().arrayCount());
    ASSERT_EQ(NewDataType::Pointer, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().subtypeAtIndex(0).subtypeAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalPointerToArrayOfInt) {
    ASTNode* node = this->parseNode("*[5]Int value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Array, node->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(1, node->dataType().subtypeAtIndex(0).subtypeCount());
    ASSERT_EQ(5, node->dataType().subtypeAtIndex(0).arrayCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().subtypeAtIndex(0).subtypeAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalFunction) {
    ASTNode* node = this->parseNode("() value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("value", node->name());
    ASSERT_EQ(NewDataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalFunctionTakingSingleParam) {
    ASTNode* node = this->parseNode("(Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalFunctionTakingTwoParams) {
    ASTNode* node = this->parseNode("(Int, Float) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(2, node->dataType().parameterCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Float, node->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalFunctionTakingAPointerParam) {
    ASTNode* node = this->parseNode("(*Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(NewDataType::Pointer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalFunctionWithReturn) {
    ASTNode* node = this->parseNode("(;Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(1, node->dataType().returnCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().returnAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalFunctionWithTwoReturns) {
    ASTNode* node = this->parseNode("(;Int, Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Function, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(2, node->dataType().returnCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().returnAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Integer, node->dataType().returnAtIndex(1).kind());
}

TEST_F(NewParserTests_Types, GlobalPointerToFunction) {
    ASTNode* node = this->parseNode("*(Int) value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Function, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalClosure) {
    ASTNode* node = this->parseNode("{} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("value", node->name());
    ASSERT_EQ(NewDataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalClosureTakingSingleParam) {
    ASTNode* node = this->parseNode("{Int} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalClosureTakingTwoParams) {
    ASTNode* node = this->parseNode("{Int, Float} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(2, node->dataType().parameterCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Float, node->dataType().parameterAtIndex(1).kind());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalClosureTakingAPointerParam) {
    ASTNode* node = this->parseNode("{*Int} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(1, node->dataType().parameterCount());
    ASSERT_EQ(NewDataType::Pointer, node->dataType().parameterAtIndex(0).kind());
    ASSERT_EQ(0, node->dataType().returnCount());
}

TEST_F(NewParserTests_Types, GlobalClosureWithReturn) {
    ASTNode* node = this->parseNode("{;Int} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(1, node->dataType().returnCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().returnAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, GlobalClosureWithTwoReturns) {
    ASTNode* node = this->parseNode("{;Int, Int} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Closure, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
    ASSERT_EQ(0, node->dataType().parameterCount());
    ASSERT_EQ(2, node->dataType().returnCount());
    ASSERT_EQ(NewDataType::Integer, node->dataType().returnAtIndex(0).kind());
    ASSERT_EQ(NewDataType::Integer, node->dataType().returnAtIndex(1).kind());
}

TEST_F(NewParserTests_Types, GlobalPointerToClosure) {
    ASTNode* node = this->parseNode("*{Int} value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Pointer, node->dataType().kind());
    ASSERT_EQ(1, node->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Closure, node->dataType().subtypeAtIndex(0).kind());
}

TEST_F(NewParserTests_Types, VarargVariable) {
    ASTNode* node = this->parseNode("Vararg value\n");

    node = node->childAtIndex(0);

    ASSERT_EQ(NewDataType::Vararg, node->dataType().kind());
    ASSERT_EQ(0, node->dataType().subtypeCount());
}
