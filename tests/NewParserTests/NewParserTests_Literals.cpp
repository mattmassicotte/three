#include "../ParserTestsBase.h"
#include "compiler/constructs/NewDataType.h"

class ParserTests_Literals : public ParserTestsBase {
};

TEST_F(ParserTests_Literals, IntegerLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = 1\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Integer Literal", node->childAtIndex(1)->nodeName());

    IntegerLiteralNode* literal = dynamic_cast<IntegerLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(1, literal->value());
    ASSERT_EQ(NewDataType::Kind::Integer, literal->dataType().kind());
    ASSERT_EQ(0, literal->dataType().widthSpecifier());
}

TEST_F(ParserTests_Literals, NegativeIntegerLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = -1\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());

    node = node->childAtIndex(1);
    ASSERT_EQ("Unary Minus Operator", node->nodeName());
    ASSERT_EQ("Integer Literal", node->childAtIndex(0)->nodeName());

    IntegerLiteralNode* literal = dynamic_cast<IntegerLiteralNode*>(node->childAtIndex(0));

    ASSERT_EQ(1, literal->value());
    ASSERT_EQ(NewDataType::Kind::Integer, literal->dataType().kind());
}

TEST_F(ParserTests_Literals, IntegerLiteralWithTypeSpecifier) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = 1:Natural:64\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Integer Literal", node->childAtIndex(1)->nodeName());

    IntegerLiteralNode* literal = dynamic_cast<IntegerLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(1, literal->value());
    ASSERT_EQ(NewDataType::Kind::Natural, literal->dataType().kind());
    ASSERT_EQ(64, literal->dataType().widthSpecifier());
}

TEST_F(ParserTests_Literals, FloatLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = 1.0\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Real Literal", node->childAtIndex(1)->nodeName());

    RealLiteralNode* literal = dynamic_cast<RealLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(1.0, literal->value());
    ASSERT_EQ(NewDataType::Kind::Float, literal->dataType().kind());
    ASSERT_EQ(0, literal->dataType().widthSpecifier());
}

TEST_F(ParserTests_Literals, FloatLiteralWithTypeSpecifier) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = 1.0:Real:64\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Real Literal", node->childAtIndex(1)->nodeName());

    RealLiteralNode* literal = dynamic_cast<RealLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(1.0, literal->value());
    ASSERT_EQ(NewDataType::Kind::Real, literal->dataType().kind());
    ASSERT_EQ(64, literal->dataType().widthSpecifier());
}

TEST_F(ParserTests_Literals, HexLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = 0x4d2\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Integer Literal", node->childAtIndex(1)->nodeName());

    IntegerLiteralNode* literal = dynamic_cast<IntegerLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(1234, literal->value());
    ASSERT_EQ(NewDataType::Kind::Integer, literal->dataType().kind());
    ASSERT_EQ(0, literal->dataType().widthSpecifier());
}

TEST_F(ParserTests_Literals, BinaryLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Int a)\n"
                                              "  a = 0b010011010010\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Integer Literal", node->childAtIndex(1)->nodeName());

    IntegerLiteralNode* literal = dynamic_cast<IntegerLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(1234, literal->value());
    ASSERT_EQ(NewDataType::Kind::Integer, literal->dataType().kind());
    ASSERT_EQ(0, literal->dataType().widthSpecifier());
}

TEST_F(ParserTests_Literals, NullLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(*Int a)\n"
                                              "  a = null\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Null Literal", node->childAtIndex(1)->nodeName());

    NullLiteralNode* literal = dynamic_cast<NullLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ(NewDataType::Kind::Pointer, literal->dataType().kind());
    ASSERT_EQ(1, literal->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Void, literal->dataType().subtypeAtIndex(0).kind());
}

TEST_F(ParserTests_Literals, StringLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(*Char a)\n"
                                              "  a = \"hello\"\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("String Literal", node->childAtIndex(1)->nodeName());

    StringLiteralNode* literal = dynamic_cast<StringLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ("hello", literal->stringValue());
    ASSERT_EQ(NewDataType::Kind::Pointer, literal->dataType().kind());
    ASSERT_EQ(1, literal->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Character, literal->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF8, literal->dataType().subtypeAtIndex(0).characterEncoding());
}

TEST_F(ParserTests_Literals, StringLiteralAscii) {
    ASTNode* node = this->parseSingleFunction("def test(*Char a)\n"
                                              "  a = \"hello\":ascii\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("String Literal", node->childAtIndex(1)->nodeName());

    StringLiteralNode* literal = dynamic_cast<StringLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ("hello", literal->stringValue());
    ASSERT_EQ(NewDataType::Kind::Pointer, literal->dataType().kind());
    ASSERT_EQ(1, literal->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Character, literal->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::ASCII, literal->dataType().subtypeAtIndex(0).characterEncoding());
}

TEST_F(ParserTests_Literals, StringLiteralUTF16LE) {
    ASTNode* node = this->parseSingleFunction("def test(*Char a)\n"
                                              "  a = \"hello\":utf16le\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("String Literal", node->childAtIndex(1)->nodeName());

    StringLiteralNode* literal = dynamic_cast<StringLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ("hello", literal->stringValue());
    ASSERT_EQ(NewDataType::Kind::Pointer, literal->dataType().kind());
    ASSERT_EQ(1, literal->dataType().subtypeCount());
    ASSERT_EQ(NewDataType::Kind::Character, literal->dataType().subtypeAtIndex(0).kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF16LE, literal->dataType().subtypeAtIndex(0).characterEncoding());
}

TEST_F(ParserTests_Literals, CharacterLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Char a)\n"
                                              "  a = 'h'\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Character Literal", node->childAtIndex(1)->nodeName());

    CharacterLiteralNode* literal = dynamic_cast<CharacterLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ("h", literal->value());
    ASSERT_EQ(NewDataType::Kind::Character, literal->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::UTF8, literal->dataType().characterEncoding());
    ASSERT_EQ(0, literal->dataType().subtypeCount());
}

TEST_F(ParserTests_Literals, CharacterLiteralAscii) {
    ASTNode* node = this->parseSingleFunction("def test(Char a)\n"
                                              "  a = 'h':ascii\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Character Literal", node->childAtIndex(1)->nodeName());

    CharacterLiteralNode* literal = dynamic_cast<CharacterLiteralNode*>(node->childAtIndex(1));

    ASSERT_EQ("h", literal->value());
    ASSERT_EQ(NewDataType::Kind::Character, literal->dataType().kind());
    ASSERT_EQ(NewDataType::CharacterEncoding::ASCII, literal->dataType().characterEncoding());
    ASSERT_EQ(0, literal->dataType().subtypeCount());
}

TEST_F(ParserTests_Literals, TrueLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Bool a)\n"
                                              "  a = true\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Boolean Literal", node->childAtIndex(1)->nodeName());

    BooleanLiteralNode* literal = dynamic_cast<BooleanLiteralNode*>(node->childAtIndex(1));

    ASSERT_TRUE(literal->value());
    ASSERT_EQ(NewDataType::Kind::Boolean, literal->dataType().kind());
    ASSERT_EQ(0, literal->dataType().subtypeCount());
}

TEST_F(ParserTests_Literals, FalseLiteral) {
    ASTNode* node = this->parseSingleFunction("def test(Bool a)\n"
                                              "  a = false\n"
                                              "end\n");

    node = node->childAtIndex(0);

    ASSERT_EQ("Assign Operator", node->nodeName());
    ASSERT_EQ("Boolean Literal", node->childAtIndex(1)->nodeName());

    BooleanLiteralNode* literal = dynamic_cast<BooleanLiteralNode*>(node->childAtIndex(1));

    ASSERT_FALSE(literal->value());
    ASSERT_EQ(NewDataType::Kind::Boolean, literal->dataType().kind());
    ASSERT_EQ(0, literal->dataType().subtypeCount());
}
