#include "compiler/Lexer.h"

#include <assert.h>
#include <gtest/gtest.h>

// This macro is a little crappy, but it needs to be careful about evaluation
#define ASSERT_TOKEN(exp_type, exp_value, token) do {\
Language::Token tmp = token; \
EXPECT_EQ(exp_type, tmp.type()); \
ASSERT_EQ(exp_value, tmp.str()); \
} while(0)

#define ASSERT_NEXT_TOKEN(exp_type, exp_value) ASSERT_TOKEN(Language::Token::Type::exp_type, exp_value, this->nextToken())

class LexerTest : public testing::Test {
protected:
    virtual void SetUp() {
        _stream = NULL;
        _lexer  = NULL;
    }

    virtual void TearDown() {
        delete _lexer;
        delete _stream;
    }

    void lex(const char* input) {
        std::string inputString(input);
        
        assert(!_stream);
        _stream = new std::istringstream(inputString);

        assert(!_lexer);
        _lexer = new Language::Lexer(_stream);
    }

    Language::Token nextToken() {
        return _lexer->nextToken();
    }
    
    std::istringstream* _stream;
    Language::Lexer* _lexer;
};

TEST_F(LexerTest, SingleIdentifierWithDigit) {
    this->lex("hell0");

    ASSERT_NEXT_TOKEN(Identifier, "hell0");
}

TEST_F(LexerTest, SingleFloatingPointValue) {
    this->lex("1234.0");

    ASSERT_NEXT_TOKEN(NumericLiteral, "1234.0");
}

TEST_F(LexerTest, SingleString) {
    this->lex("\"hello UTF8\\n: \\u8023\"");

    ASSERT_NEXT_TOKEN(String, "hello UTF8\\n: \\u8023");
}

TEST_F(LexerTest, HelloWorldProgram) {
    this->lex("def main(int argc, **char argv; int)\n    printf(\"hello world\")\n end\n");

    ASSERT_NEXT_TOKEN(KeywordDef,  "def");
    ASSERT_NEXT_TOKEN(Identifier,  "main");
    ASSERT_NEXT_TOKEN(Punctuation, "(");
    ASSERT_NEXT_TOKEN(Identifier,  "int");
    ASSERT_NEXT_TOKEN(Identifier,  "argc");
    ASSERT_NEXT_TOKEN(Punctuation, ",");
    ASSERT_NEXT_TOKEN(Punctuation, "*");
    ASSERT_NEXT_TOKEN(Punctuation, "*");
    ASSERT_NEXT_TOKEN(Identifier,  "char");
    ASSERT_NEXT_TOKEN(Identifier,  "argv");
    ASSERT_NEXT_TOKEN(Punctuation, ";");
    ASSERT_NEXT_TOKEN(Identifier,  "int");
    ASSERT_NEXT_TOKEN(Punctuation, ")");
    ASSERT_NEXT_TOKEN(Newline,     "");
    ASSERT_NEXT_TOKEN(Identifier,  "printf");
    ASSERT_NEXT_TOKEN(Punctuation, "(");
    ASSERT_NEXT_TOKEN(String,      "hello world");
    ASSERT_NEXT_TOKEN(Punctuation, ")");
    ASSERT_NEXT_TOKEN(Newline,     "");
    ASSERT_NEXT_TOKEN(KeywordEnd,  "end");
    ASSERT_NEXT_TOKEN(Newline,     "");
    ASSERT_NEXT_TOKEN(EndOfInput,  "");
}

TEST_F(LexerTest, Comment) {
    this->lex("abc // some comment\ndf");

    ASSERT_NEXT_TOKEN(Identifier,  "abc");
    ASSERT_NEXT_TOKEN(Comment,     "// some comment");
    ASSERT_NEXT_TOKEN(Identifier,  "df");
}

TEST_F(LexerTest, HexNumericLiteral) {
    this->lex("0x4d2");

    ASSERT_NEXT_TOKEN(NumericLiteral,  "0x4d2");
}

TEST_F(LexerTest, BinaryNumericLiteral) {
    this->lex("0b010011010010");

    ASSERT_NEXT_TOKEN(NumericLiteral,  "0b010011010010");
}

TEST_F(LexerTest, ScientificNotationNumericLiteral) {
    this->lex("1.23e-2");

    ASSERT_NEXT_TOKEN(NumericLiteral,  "1.23e-2");
}

TEST_F(LexerTest, Annotation) {
    this->lex("@common");

    ASSERT_NEXT_TOKEN(Annotation,  "@common");
}
