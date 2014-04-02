#include "compiler/Lexer/SubtokenLexer.h"

#include <assert.h>
#include <gtest/gtest.h>

#define ASSERT_NEXT_SUBTOKEN(exp_type, exp_value) do {\
Three::Subtoken tmp = this->next(); \
EXPECT_EQ(Three::Subtoken::Type::exp_type, tmp.type()); \
ASSERT_EQ(exp_value, tmp.str()); \
} while(0)

class SubtokenLexerTest : public testing::Test {
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
        _lexer = new Three::SubtokenLexer(_stream);
    }

    Three::Subtoken next() {
        return _lexer->next();
    }

private:
    std::istringstream* _stream;
    Three::SubtokenLexer* _lexer;
};

TEST_F(SubtokenLexerTest, AllLetterWord) {
    this->lex("hello");

    ASSERT_NEXT_SUBTOKEN(Word, "hello");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, UnderscoredWord) {
    this->lex("hell_o");

    ASSERT_NEXT_SUBTOKEN(Word, "hell_o");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, SuccessiveAllLetterWords) {
    this->lex("hello goodbye");

    ASSERT_NEXT_SUBTOKEN(Word, "hello");
    ASSERT_NEXT_SUBTOKEN(Whitespace, " ");
    ASSERT_NEXT_SUBTOKEN(Word, "goodbye");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, WhitespaceSpan) {
    this->lex("   foo");

    ASSERT_NEXT_SUBTOKEN(Whitespace, "   ");
    ASSERT_NEXT_SUBTOKEN(Word, "foo");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, AlphanumericWord) {
    this->lex("hell0");

    ASSERT_NEXT_SUBTOKEN(Word, "hell0");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, LeadingNumberWord) {
    this->lex("12foo");

    ASSERT_NEXT_SUBTOKEN(Word, "12foo");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, Newline) {
    this->lex("\n");

    ASSERT_NEXT_SUBTOKEN(Newline, "\n");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, WordsSeperatedByNewlines) {
    this->lex("foo\n\nbar\n");

    ASSERT_NEXT_SUBTOKEN(Word, "foo");
    ASSERT_NEXT_SUBTOKEN(Newline, "\n");
    ASSERT_NEXT_SUBTOKEN(Newline, "\n");
    ASSERT_NEXT_SUBTOKEN(Word, "bar");
    ASSERT_NEXT_SUBTOKEN(Newline, "\n");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, Punctuation) {
    this->lex("~!@%^&*()-+={}[]|/?<>.,:;");

    ASSERT_NEXT_SUBTOKEN(Punctuation, "~");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "!");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "@");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "%");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "^");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "&");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "*");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "(");
    ASSERT_NEXT_SUBTOKEN(Punctuation, ")");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "-");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "+");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "=");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "{");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "}");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "[");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "]");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "|");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "/");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "?");
    ASSERT_NEXT_SUBTOKEN(Punctuation, "<");
    ASSERT_NEXT_SUBTOKEN(Punctuation, ">");
    ASSERT_NEXT_SUBTOKEN(Punctuation, ".");
    ASSERT_NEXT_SUBTOKEN(Punctuation, ",");
    ASSERT_NEXT_SUBTOKEN(Punctuation, ":");
    ASSERT_NEXT_SUBTOKEN(Punctuation, ";");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, Comment) {
    this->lex("# abc def\n");

    ASSERT_NEXT_SUBTOKEN(Comment, "# abc def");
    ASSERT_NEXT_SUBTOKEN(Newline, "\n");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, WordFollowedByComment) {
    this->lex("abc#def\n");

    ASSERT_NEXT_SUBTOKEN(Word, "abc");
    ASSERT_NEXT_SUBTOKEN(Comment, "#def");
    ASSERT_NEXT_SUBTOKEN(Newline, "\n");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, String) {
    this->lex("\"abc\"");

    ASSERT_NEXT_SUBTOKEN(String, "\"abc\"");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, Character) {
    this->lex("'a'");

    ASSERT_NEXT_SUBTOKEN(Character, "'a'");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, Number) {
    this->lex("1234567890");

    ASSERT_NEXT_SUBTOKEN(Number, "1234567890");
    ASSERT_NEXT_SUBTOKEN(EndOfInput, "");
}

TEST_F(SubtokenLexerTest, IfDirective) {
    this->lex("%if abc");

    ASSERT_NEXT_SUBTOKEN(DirectiveIf, "if");
    ASSERT_NEXT_SUBTOKEN(Whitespace, " ");
    ASSERT_NEXT_SUBTOKEN(Word, "abc");
}

TEST_F(SubtokenLexerTest, ElseDirective) {
    this->lex("%else");

    ASSERT_NEXT_SUBTOKEN(DirectiveElse, "else");
}

TEST_F(SubtokenLexerTest, EndDirective) {
    this->lex("%end");

    ASSERT_NEXT_SUBTOKEN(DirectiveEnd, "end");
}
