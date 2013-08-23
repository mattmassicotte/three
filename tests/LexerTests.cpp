#include "compiler/Lexer.h"

#include <assert.h>
#include <gtest/gtest.h>

// This macro is a little crappy, but it needs to be careful about evaluation
#define ASSERT_TOKEN(exp_type, exp_value, token) do {\
Language::Token tmp = token; \
EXPECT_EQ(exp_type, tmp.type()); \
ASSERT_EQ(exp_value, tmp.str()); \
} while(0)

#define ASSERT_NEXT_TOKEN(exp_type, exp_value) ASSERT_TOKEN(Language::Token::Type::exp_type, exp_value, this->next())

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

    Language::Token next() {
        return _lexer->next();
    }

    Language::Token peek(unsigned int distance = 1) {
        return _lexer->peek(distance);
    }

    std::istringstream* _stream;
    Language::Lexer*    _lexer;
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
    this->lex("def main(Int argc, **Char argv; Int)\n    printf(\"hello world\")\n end\n");

    ASSERT_NEXT_TOKEN(KeywordDef,  "def");
    ASSERT_NEXT_TOKEN(Identifier,  "main");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen, "(");
    ASSERT_NEXT_TOKEN(Identifier,  "Int");
    ASSERT_NEXT_TOKEN(Identifier,  "argc");
    ASSERT_NEXT_TOKEN(Punctuation, ",");
    ASSERT_NEXT_TOKEN(Operator,    "*");
    ASSERT_NEXT_TOKEN(Operator,    "*");
    ASSERT_NEXT_TOKEN(Identifier,  "Char");
    ASSERT_NEXT_TOKEN(Identifier,  "argv");
    ASSERT_NEXT_TOKEN(PunctuationSemicolon,  ";");
    ASSERT_NEXT_TOKEN(Identifier,  "Int");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
    ASSERT_NEXT_TOKEN(Newline,     "");
    ASSERT_NEXT_TOKEN(Identifier,  "printf");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen, "(");
    ASSERT_NEXT_TOKEN(String,      "hello world");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
    ASSERT_NEXT_TOKEN(Newline,     "");
    ASSERT_NEXT_TOKEN(KeywordEnd,  "end");
    ASSERT_NEXT_TOKEN(Newline,     "");
    ASSERT_NEXT_TOKEN(EndOfInput,  "");
}

TEST_F(LexerTest, Comment) {
    this->lex("abc // some comment\ndf");

    ASSERT_NEXT_TOKEN(Identifier, "abc");
    ASSERT_NEXT_TOKEN(Newline,    "");
    ASSERT_NEXT_TOKEN(Identifier, "df");
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

TEST_F(LexerTest, LookAhead) {
    this->lex("hell0 goodbye yes no");

    ASSERT_EQ("goodbye", this->peek(2).str());
    ASSERT_EQ("no", this->peek(4).str());
    ASSERT_EQ("yes", this->peek(3).str());
    ASSERT_EQ("hell0", this->peek(1).str());
}

TEST_F(LexerTest, Semicolon) {
    this->lex(";");

    ASSERT_NEXT_TOKEN(PunctuationSemicolon,  ";");
}

TEST_F(LexerTest, UnaryStarOperator) {
    this->lex("*a");

    ASSERT_NEXT_TOKEN(Operator,       "*");
    ASSERT_NEXT_TOKEN(Identifier,     "a");
}

TEST_F(LexerTest, UnaryAndOperator) {
    this->lex("&a");

    ASSERT_NEXT_TOKEN(Operator,       "&");
    ASSERT_NEXT_TOKEN(Identifier,     "a");
}

TEST_F(LexerTest, AlignofOperator) {
    this->lex("alignof(a)");

    ASSERT_NEXT_TOKEN(KeywordAlignof,       "alignof");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen, "(");
    ASSERT_NEXT_TOKEN(Identifier,           "a");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
}

TEST_F(LexerTest, OffsetofOperator) {
    this->lex("offsetof(a)");

    ASSERT_NEXT_TOKEN(KeywordOffsetof,      "offsetof");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen, "(");
    ASSERT_NEXT_TOKEN(Identifier,           "a");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
}

TEST_F(LexerTest, TypeofOperator) {
    this->lex("typeof(a)");

    ASSERT_NEXT_TOKEN(KeywordTypeof,        "typeof");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen, "(");
    ASSERT_NEXT_TOKEN(Identifier,           "a");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
}

TEST_F(LexerTest, UnaryMinusOperator) {
    this->lex("-a");

    ASSERT_NEXT_TOKEN(Operator,             "-");
    ASSERT_NEXT_TOKEN(Identifier,           "a");
}

TEST_F(LexerTest, PreincrementOperator) {
    this->lex("++a");

    ASSERT_NEXT_TOKEN(Operator,   "++");
    ASSERT_NEXT_TOKEN(Identifier, "a");
}

TEST_F(LexerTest, PostincrementOperator) {
    this->lex("a++");

    ASSERT_NEXT_TOKEN(Identifier,   "a");
    ASSERT_NEXT_TOKEN(Operator, "++");
}

TEST_F(LexerTest, PredecrementOperator) {
    this->lex("--a");

    ASSERT_NEXT_TOKEN(Operator,   "--");
    ASSERT_NEXT_TOKEN(Identifier, "a");
}

TEST_F(LexerTest, PostdecrementOperator) {
    this->lex("a--");

    ASSERT_NEXT_TOKEN(Identifier,   "a");
    ASSERT_NEXT_TOKEN(Operator, "--");
}

TEST_F(LexerTest, BinaryAdditionOperator) {
    this->lex("a + b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "+");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BinarySubtractionOperator) {
    this->lex("a - b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "-");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BinaryDivisionOperator) {
    this->lex("a / b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "/");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BinaryModulusOperator) {
    this->lex("a % b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "%");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BinaryMultiplicationOperator) {
    this->lex("a * b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "*");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, EqualityOperator) {
    this->lex("a == b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "==");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, DeepEqualityOperator) {
    this->lex("a === 1");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "===");
    ASSERT_NEXT_TOKEN(NumericLiteral, "1");
}

TEST_F(LexerTest, ComparisonOperator) {
    this->lex("a <=> b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "<=>");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, InequalityOperator) {
    this->lex("a != b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "!=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

// TEST_F(LexerTest, SingleInequalityOperator) {
//     this->lex("a ≠ b");
// 
//     ASSERT_NEXT_TOKEN(Identifier, "a");
//     ASSERT_NEXT_TOKEN(Operator,   "≠");
//     ASSERT_NEXT_TOKEN(Identifier, "b");
// }

TEST_F(LexerTest, GreaterThanOperator) {
    this->lex("a > b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   ">");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, LessThanOperator) {
    this->lex("a < b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "<");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, GreaterThanOrEqualOperator) {
    this->lex("a >= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   ">=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

// TEST_F(LexerTest, SingleGreaterThanOrEqualOperator) {
//     this->lex("a ≥ b");
// 
//     ASSERT_NEXT_TOKEN(Identifier, "a");
//     ASSERT_NEXT_TOKEN(Operator,   "≥");
//     ASSERT_NEXT_TOKEN(Identifier, "b");
// }

TEST_F(LexerTest, LessThanOrEqualOperator) {
    this->lex("a <= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "<=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

// TEST_F(LexerTest, SingleLessThanOrEqualOperator) {
//     this->lex("a ≤ b");
// 
//     ASSERT_NEXT_TOKEN(Identifier, "a");
//     ASSERT_NEXT_TOKEN(Operator,   "≤");
//     ASSERT_NEXT_TOKEN(Identifier, "b");
// }

TEST_F(LexerTest, BooleanAndOperator) {
    this->lex("a && b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "&&");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompositionAndOperator) {
    this->lex("a and b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "and");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BooleanOrOperator) {
    this->lex("a || b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "||");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompositionOrOperator) {
    this->lex("a or b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "or");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BooleanXorOperator) {
    this->lex("a ^^ b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "^^");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BooleanNotOperator) {
    this->lex("!a");

    ASSERT_NEXT_TOKEN(Operator,   "!");
    ASSERT_NEXT_TOKEN(Identifier, "a");
}

TEST_F(LexerTest, BitwiseNotOperator) {
    this->lex("~a");

    ASSERT_NEXT_TOKEN(Operator,   "~");
    ASSERT_NEXT_TOKEN(Identifier, "a");
}

TEST_F(LexerTest, BitwiseAndOperator) {
    this->lex("a & b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "&");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BitwiseOrOperator) {
    this->lex("a | b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "|");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BitwiseXorOperator) {
    this->lex("a ^ b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "^");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BitwiseLeftShiftOperator) {
    this->lex("a << b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "<<");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, BitwiseRightShiftOperator) {
    this->lex("a >> b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   ">>");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, AssignmentOperator) {
    this->lex("a = b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundAdditionAssignmntOperator) {
    this->lex("a += b");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "+=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundSubtractionsAssignmntOperator) {
    this->lex("a -= b");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "-=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundMultiplicationAssignmntOperator) {
    this->lex("a *= b");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "*=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundDivideAssignOperator) {
    this->lex("a /= b");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "/=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundModulusAssignOperator) {
    this->lex("a %= b");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "%=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundBitwiseAndAssignOperator) {
    this->lex("a &= b");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "&=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundBitwiseOrAssignOperator) {
    this->lex("a |= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "|=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundBitwiseXorAssignOperator) {
    this->lex("a ^= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "^=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundLeftShiftAssignOperator) {
    this->lex("a <<= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "<<=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundRightShiftAssignOperator) {
    this->lex("a >>= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   ">>=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundAndAssignOperator) {
    this->lex("a &&= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "&&=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, CompoundOrAssignOperator) {
    this->lex("a ||= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "||=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
}

TEST_F(LexerTest, TernaryConditionalOperator) {
    this->lex("a ? b : c");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "?");
    ASSERT_NEXT_TOKEN(Identifier, "b");
    ASSERT_NEXT_TOKEN(PunctuationColon, ":");
    ASSERT_NEXT_TOKEN(Identifier, "c");
}

TEST_F(LexerTest, TernaryCompareAndSwapOperator) {
    this->lex("a cas b : c");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Operator,   "cas");
    ASSERT_NEXT_TOKEN(Identifier, "b");
    ASSERT_NEXT_TOKEN(PunctuationColon, ":");
    ASSERT_NEXT_TOKEN(Identifier, "c");
}

TEST_F(LexerTest, AssignmentFollowedByUnaryOperator) {
    this->lex("a = &1");

    ASSERT_NEXT_TOKEN(Identifier,     "a");
    ASSERT_NEXT_TOKEN(Operator,       "=");
    ASSERT_NEXT_TOKEN(Operator,       "&");
    ASSERT_NEXT_TOKEN(NumericLiteral, "1");
}

TEST_F(LexerTest, VarargType) {
    this->lex("Vararg ap");

    ASSERT_NEXT_TOKEN(KeywordVararg, "Vararg");
    ASSERT_NEXT_TOKEN(Identifier,    "ap");
}

TEST_F(LexerTest, VarargNext) {
    this->lex("nextarg:Int");

    ASSERT_NEXT_TOKEN(KeywordVarargNext, "nextarg");
    ASSERT_NEXT_TOKEN(PunctuationColon,  ":");
    ASSERT_NEXT_TOKEN(Identifier,        "Int");
}

TEST_F(LexerTest, IfKeyword) {
    this->lex("if a");

    ASSERT_NEXT_TOKEN(KeywordIf, "if");
    ASSERT_NEXT_TOKEN(Identifier, "a");
}

TEST_F(LexerTest, UnlessKeyword) {
    this->lex("a unless b");

    ASSERT_NEXT_TOKEN(Identifier,    "a");
    ASSERT_NEXT_TOKEN(KeywordUnless, "unless");
    ASSERT_NEXT_TOKEN(Identifier,    "b");
}

TEST_F(LexerTest, SwitchAndCaseKeywords) {
    this->lex("switch a\ncase b\nend");

    ASSERT_NEXT_TOKEN(KeywordSwitch, "switch");
    ASSERT_NEXT_TOKEN(Identifier,    "a");
    ASSERT_NEXT_TOKEN(Newline,       "");
    ASSERT_NEXT_TOKEN(KeywordCase,   "case");
    ASSERT_NEXT_TOKEN(Identifier,    "b");
    ASSERT_NEXT_TOKEN(Newline,       "");
    ASSERT_NEXT_TOKEN(KeywordEnd,    "end");
}

TEST_F(LexerTest, LoopKeywords) {
    this->lex("loop a\nbreak\ncontinue\nend");

    ASSERT_NEXT_TOKEN(KeywordLoop,     "loop");
    ASSERT_NEXT_TOKEN(Identifier,      "a");
    ASSERT_NEXT_TOKEN(Newline,         "");
    ASSERT_NEXT_TOKEN(KeywordBreak,    "break");
    ASSERT_NEXT_TOKEN(Newline,         "");
    ASSERT_NEXT_TOKEN(KeywordContinue, "continue");
    ASSERT_NEXT_TOKEN(Newline,         "");
    ASSERT_NEXT_TOKEN(KeywordEnd,      "end");
}

TEST_F(LexerTest, ForKeywords) {
    this->lex("for (Int a = 0; a < 5; a++)\nend");

    ASSERT_NEXT_TOKEN(KeywordFor,     "for");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen,  "(");
    ASSERT_NEXT_TOKEN(Identifier,      "Int");
    ASSERT_NEXT_TOKEN(Identifier,      "a");
    ASSERT_NEXT_TOKEN(Operator,       "=");
    ASSERT_NEXT_TOKEN(NumericLiteral,       "0");
    ASSERT_NEXT_TOKEN(PunctuationSemicolon,       ";");
    ASSERT_NEXT_TOKEN(Identifier,      "a");
    ASSERT_NEXT_TOKEN(Operator,      "<");
    ASSERT_NEXT_TOKEN(NumericLiteral,       "5");
    ASSERT_NEXT_TOKEN(PunctuationSemicolon,       ";");
    ASSERT_NEXT_TOKEN(Identifier,      "a");
    ASSERT_NEXT_TOKEN(Operator,       "++");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen,  ")");
    ASSERT_NEXT_TOKEN(Newline,         "");
    ASSERT_NEXT_TOKEN(KeywordEnd,      "end");
}

TEST_F(LexerTest, LinkageKeyword) {
    this->lex("linkage(none)");

    ASSERT_NEXT_TOKEN(KeywordLinkage,        "linkage");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen,  "(");
    ASSERT_NEXT_TOKEN(Identifier,            "none");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
}

TEST_F(LexerTest, AbiKeyword) {
    this->lex("abi(cdecls)");

    ASSERT_NEXT_TOKEN(KeywordAbi,            "abi");
    ASSERT_NEXT_TOKEN(PunctuationOpenParen,  "(");
    ASSERT_NEXT_TOKEN(Identifier,            "cdecls");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen, ")");
}

TEST_F(LexerTest, DebugKeyword) {
    this->lex("debug\n");

    ASSERT_NEXT_TOKEN(KeywordDebug, "debug");
    ASSERT_NEXT_TOKEN(Newline,      "");
}

TEST_F(LexerTest, VersionKeyword) {
    this->lex("version something");

    ASSERT_NEXT_TOKEN(KeywordVersion, "version");
    ASSERT_NEXT_TOKEN(Identifier,     "something");
}

TEST_F(LexerTest, NamespaceKeyword) {
    this->lex("namespace Foo");

    ASSERT_NEXT_TOKEN(KeywordNamespace, "namespace");
    ASSERT_NEXT_TOKEN(Identifier,       "Foo");
}
