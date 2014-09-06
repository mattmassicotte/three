#include "compiler/Lexer/Lexer.h"

#include <assert.h>
#include <gtest/gtest.h>

#define ASSERT_NEXT_TOKEN(exp_type, exp_value) do {\
Three::Token tmp = this->next(); \
EXPECT_EQ(Three::Token::Type::exp_type, tmp.type()); \
ASSERT_EQ(exp_value, tmp.str()); \
} while(0)

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
        _lexer = new Three::Lexer(_stream);
    }

    Three::Token next() {
        return _lexer->next();
    }

    Three::Lexer* lexer() const {
        return _lexer;
    }

private:
    std::istringstream* _stream;
    Three::Lexer* _lexer;
};

TEST_F(LexerTest, Comment) {
    this->lex("abc # some comment\ndf");

    ASSERT_NEXT_TOKEN(Identifier, "abc");
    ASSERT_NEXT_TOKEN(Newline,    "\n");
    ASSERT_NEXT_TOKEN(Identifier, "df");
}

TEST_F(LexerTest, SingleIdentifierWithDigit) {
    this->lex("hell0");

    ASSERT_NEXT_TOKEN(Identifier, "hell0");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// Punctuation
TEST_F(LexerTest, Semicolon) {
    this->lex(";");

    ASSERT_NEXT_TOKEN(PunctuationSemicolon,  ";");
}

TEST_F(LexerTest, Colon) {
    this->lex(":");

    ASSERT_NEXT_TOKEN(PunctuationColon,  ":");
}

TEST_F(LexerTest, Comma) {
    this->lex(",");

    ASSERT_NEXT_TOKEN(PunctuationComma,  ",");
}

TEST_F(LexerTest, Braces) {
    this->lex("{}");

    ASSERT_NEXT_TOKEN(PunctuationOpenBrace,  "{");
    ASSERT_NEXT_TOKEN(PunctuationCloseBrace,  "}");
}

TEST_F(LexerTest, Brackets) {
    this->lex("[]");

    ASSERT_NEXT_TOKEN(PunctuationOpenBracket,  "[");
    ASSERT_NEXT_TOKEN(PunctuationCloseBracket,  "]");
}

TEST_F(LexerTest, Parens) {
    this->lex("()");

    ASSERT_NEXT_TOKEN(PunctuationOpenParen,  "(");
    ASSERT_NEXT_TOKEN(PunctuationCloseParen,  ")");
}

// Literals
TEST_F(LexerTest, SingleString) {
    this->lex("\"hello UTF8\\n: \\u8023\"");

    ASSERT_NEXT_TOKEN(LiteralString, "\"hello UTF8\\n: \\u8023\"");
}

TEST_F(LexerTest, LiteralInteger) {
    this->lex("1234");

    ASSERT_NEXT_TOKEN(LiteralInteger, "1234");
}

TEST_F(LexerTest, BinaryNumber) {
    this->lex("0b010011010010");

    ASSERT_NEXT_TOKEN(LiteralBinary,  "0b010011010010");
}

TEST_F(LexerTest, HexNumber) {
    this->lex("0x4d2");

    ASSERT_NEXT_TOKEN(LiteralHex, "0x4d2");
}

TEST_F(LexerTest, SingleFloatingPointValue) {
    this->lex("1234.0");

    ASSERT_NEXT_TOKEN(LiteralReal, "1234.0");
}

TEST_F(LexerTest, TrueLiteral) {
    this->lex("true");

    ASSERT_NEXT_TOKEN(LiteralTrue, "true");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, FalseLiteral) {
    this->lex("false");

    ASSERT_NEXT_TOKEN(LiteralFalse, "false");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, NullLiteral) {
    this->lex("null");

    ASSERT_NEXT_TOKEN(LiteralNull,  "null");
}

TEST_F(LexerTest, CharacterLiteral) {
    this->lex("'c'");

    ASSERT_NEXT_TOKEN(LiteralCharacter,  "'c'");
}

// Operators
TEST_F(LexerTest, OperatorStar) {
    this->lex("*");

    ASSERT_NEXT_TOKEN(OperatorStar, "*");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorAmpersand) {
    this->lex("&");

    ASSERT_NEXT_TOKEN(OperatorAmpersand, "&");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorMinus) {
    this->lex("-");

    ASSERT_NEXT_TOKEN(OperatorMinus, "-");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorIncrement) {
    this->lex("++");

    ASSERT_NEXT_TOKEN(OperatorIncrement, "++");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorDecrement) {
    this->lex("--");

    ASSERT_NEXT_TOKEN(OperatorDecrement, "--");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorPlus) {
    this->lex("+");

    ASSERT_NEXT_TOKEN(OperatorPlus, "+");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorDivide) {
    this->lex("/");

    ASSERT_NEXT_TOKEN(OperatorDivide, "/");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorMod) {
    // This one is slightly special, because a starting % means directive
    this->lex("a % b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(OperatorMod, "%");
    ASSERT_NEXT_TOKEN(Identifier, "b");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorDot) {
    this->lex(".");

    ASSERT_NEXT_TOKEN(OperatorDot, ".");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, ArrowOperator) {
    this->lex("a->b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(OperatorArrow, "->");
    ASSERT_NEXT_TOKEN(Identifier, "b");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// Comparison Operators
TEST_F(LexerTest, OperatorEqual) {
    this->lex("==");

    ASSERT_NEXT_TOKEN(OperatorEqual, "==");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorDeepEqual) {
    this->lex("===");

    ASSERT_NEXT_TOKEN(OperatorDeepEqual, "===");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorCompare) {
    this->lex("<=>");

    ASSERT_NEXT_TOKEN(OperatorCompare, "<=>");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorNotEqual) {
    this->lex("!=");

    ASSERT_NEXT_TOKEN(OperatorNotEqual, "!=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorGreaterThan) {
    this->lex(">");

    ASSERT_NEXT_TOKEN(OperatorGreaterThan, ">");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLessThan) {
    this->lex("<");

    ASSERT_NEXT_TOKEN(OperatorLessThan, "<");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorGreaterOrEqual) {
    this->lex(">=");

    ASSERT_NEXT_TOKEN(OperatorGreaterOrEqual, ">=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLessOrEqual) {
    this->lex("<=");

    ASSERT_NEXT_TOKEN(OperatorLessOrEqual, "<=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLogicalAnd) {
    this->lex("&&");

    ASSERT_NEXT_TOKEN(OperatorLogicalAnd, "&&");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLogicalOr) {
    this->lex("||");

    ASSERT_NEXT_TOKEN(OperatorLogicalOr, "||");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLogicalXor) {
    this->lex("^^");

    ASSERT_NEXT_TOKEN(OperatorLogicalXor, "^^");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorNot) {
    this->lex("!");

    ASSERT_NEXT_TOKEN(OperatorNot, "!");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorBinaryNot) {
    this->lex("~");

    ASSERT_NEXT_TOKEN(OperatorBinaryNot, "~");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorBinaryOr) {
    this->lex("|");

    ASSERT_NEXT_TOKEN(OperatorBinaryOr, "|");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorBinaryXor) {
    this->lex("^");

    ASSERT_NEXT_TOKEN(OperatorBinaryXor, "^");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLeftShift) {
    this->lex("<<");

    ASSERT_NEXT_TOKEN(OperatorLeftShift, "<<");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorRightShift) {
    this->lex(">>");

    ASSERT_NEXT_TOKEN(OperatorRightShift, ">>");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// assignment
TEST_F(LexerTest, OperatorAssign) {
    this->lex(">>");

    ASSERT_NEXT_TOKEN(OperatorRightShift, ">>");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorAddAssign) {
    this->lex("+=");

    ASSERT_NEXT_TOKEN(OperatorAddAssign, "+=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorSubtractAssign) {
    this->lex("-=");

    ASSERT_NEXT_TOKEN(OperatorSubtractAssign, "-=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorMultiplyAssign) {
    this->lex("*=");

    ASSERT_NEXT_TOKEN(OperatorMultiplyAssign, "*=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorDivideAssign) {
    this->lex("/=");

    ASSERT_NEXT_TOKEN(OperatorDivideAssign, "/=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorModAssign) {
    // avoid directive
    this->lex("a %= b");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(OperatorModAssign, "%=");
    ASSERT_NEXT_TOKEN(Identifier, "b");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorBitwiseAndAssign) {
    this->lex("&=");

    ASSERT_NEXT_TOKEN(OperatorBitwiseAndAssign, "&=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorBitwiseOrAssign) {
    this->lex("|=");

    ASSERT_NEXT_TOKEN(OperatorBitwiseOrAssign, "|=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorBitwiseXorAssign) {
    this->lex("^=");

    ASSERT_NEXT_TOKEN(OperatorBitwiseXorAssign, "^=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLeftShiftAssign) {
    this->lex("<<=");

    ASSERT_NEXT_TOKEN(OperatorLeftShiftAssign, "<<=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorRightShiftAssign) {
    this->lex(">>=");

    ASSERT_NEXT_TOKEN(OperatorRightShiftAssign, ">>=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLogicalAndAssign) {
    this->lex("&&=");

    ASSERT_NEXT_TOKEN(OperatorLogicalAndAssign, "&&=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorLogicalOrAssign) {
    this->lex("||=");

    ASSERT_NEXT_TOKEN(OperatorLogicalOrAssign, "||=");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// Terary Ops
TEST_F(LexerTest, OperatorQuestionMark) {
    this->lex("?");

    ASSERT_NEXT_TOKEN(OperatorQuestionMark, "?");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, OperatorCAS) {
    this->lex("cas");

    ASSERT_NEXT_TOKEN(OperatorCAS, "cas");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// meta-functions
TEST_F(LexerTest, MetafunctionSizeOf) {
    this->lex("sizeof");

    ASSERT_NEXT_TOKEN(MetafunctionSizeOf, "sizeof");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionCast) {
    this->lex("cast");

    ASSERT_NEXT_TOKEN(MetafunctionCast, "cast");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionAlignof) {
    this->lex("alignof");

    ASSERT_NEXT_TOKEN(MetafunctionAlignof, "alignof");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionOffsetof) {
    this->lex("offsetof");

    ASSERT_NEXT_TOKEN(MetafunctionOffsetof, "offsetof");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionTypeof) {
    this->lex("typeof");

    ASSERT_NEXT_TOKEN(MetafunctionTypeof, "typeof");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionNameof) {
    this->lex("nameof");

    ASSERT_NEXT_TOKEN(MetafunctionNameof, "nameof");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionCardinalityof) {
    this->lex("cardinalityof");

    ASSERT_NEXT_TOKEN(MetafunctionCardinalityof, "cardinalityof");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordDef) {
    this->lex("def");

    ASSERT_NEXT_TOKEN(KeywordDef, "def");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordEnd) {
    this->lex("end");

    ASSERT_NEXT_TOKEN(KeywordEnd, "end");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordReturn) {
    this->lex("return");

    ASSERT_NEXT_TOKEN(KeywordReturn, "return");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordEnsure) {
    this->lex("ensure");

    ASSERT_NEXT_TOKEN(KeywordEnsure, "ensure");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordIf) {
    this->lex("if");

    ASSERT_NEXT_TOKEN(KeywordIf, "if");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordElse) {
    this->lex("else");

    ASSERT_NEXT_TOKEN(KeywordElse, "else");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordUnless) {
    this->lex("unless");

    ASSERT_NEXT_TOKEN(KeywordUnless, "unless");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordLoop) {
    this->lex("loop");

    ASSERT_NEXT_TOKEN(KeywordLoop, "loop");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordFor) {
    this->lex("for");

    ASSERT_NEXT_TOKEN(KeywordFor, "for");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordBreak) {
    this->lex("break");

    ASSERT_NEXT_TOKEN(KeywordBreak, "break");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordContinue) {
    this->lex("continue");

    ASSERT_NEXT_TOKEN(KeywordContinue, "continue");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordImport) {
    this->lex("import");

    ASSERT_NEXT_TOKEN(KeywordImport, "import");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordClosure) {
    this->lex("do");

    ASSERT_NEXT_TOKEN(KeywordClosure, "do");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordStructure) {
    this->lex("struct");

    ASSERT_NEXT_TOKEN(KeywordStructure, "struct");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordEnumeration) {
    this->lex("enum");

    ASSERT_NEXT_TOKEN(KeywordEnumeration, "enum");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordUnion) {
    this->lex("union");

    ASSERT_NEXT_TOKEN(KeywordUnion, "union");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordSwitch) {
    this->lex("switch");

    ASSERT_NEXT_TOKEN(KeywordSwitch, "switch");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordCase) {
    this->lex("case");

    ASSERT_NEXT_TOKEN(KeywordCase, "case");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordAtomic) {
    this->lex("atomic");

    ASSERT_NEXT_TOKEN(KeywordAtomic, "atomic");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordBarrier) {
    this->lex("barrier");

    ASSERT_NEXT_TOKEN(KeywordBarrier, "barrier");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordAbort) {
    this->lex("abort");

    ASSERT_NEXT_TOKEN(KeywordAbort, "abort");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordVararg) {
    this->lex("Vararg");

    ASSERT_NEXT_TOKEN(KeywordVararg, "Vararg");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, MetafunctionNextarg) {
    this->lex("nextarg");

    ASSERT_NEXT_TOKEN(MetafunctionNextarg, "nextarg");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordPublic) {
    this->lex("public");

    ASSERT_NEXT_TOKEN(KeywordPublic, "public");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordPrivate) {
    this->lex("private");

    ASSERT_NEXT_TOKEN(KeywordPrivate, "private");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordNamespace) {
    this->lex("namespace");

    ASSERT_NEXT_TOKEN(KeywordNamespace, "namespace");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordModule) {
    this->lex("module");

    ASSERT_NEXT_TOKEN(KeywordModule, "module");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordInclude) {
    this->lex("include");

    ASSERT_NEXT_TOKEN(KeywordInclude, "include");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordLinkage) {
    this->lex("linkage");

    ASSERT_NEXT_TOKEN(KeywordLinkage, "linkage");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordABI) {
    this->lex("abi");

    ASSERT_NEXT_TOKEN(KeywordABI, "abi");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, KeywordAssert) {
    this->lex("assert");

    ASSERT_NEXT_TOKEN(KeywordAssert, "assert");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// Annotations
TEST_F(LexerTest, AnnotationAccess) {
    this->lex("@access");

    ASSERT_NEXT_TOKEN(AnnotationAccess, "@access");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationVolatile) {
    this->lex("@volatile");

    ASSERT_NEXT_TOKEN(AnnotationVolatile, "@volatile");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationAlias) {
    this->lex("@alias");

    ASSERT_NEXT_TOKEN(AnnotationAlias, "@alias");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationRead) {
    this->lex("@read");

    ASSERT_NEXT_TOKEN(AnnotationRead, "@read");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationWrite) {
    this->lex("@write");

    ASSERT_NEXT_TOKEN(AnnotationWrite, "@write");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationAssert) {
    this->lex("@assert");

    ASSERT_NEXT_TOKEN(AnnotationAssert, "@assert");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationSentinel) {
    this->lex("@sentinel");

    ASSERT_NEXT_TOKEN(AnnotationSentinel, "@sentinel");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationChecked) {
    this->lex("@checked");

    ASSERT_NEXT_TOKEN(AnnotationChecked, "@checked");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationNull) {
    this->lex("@null");

    ASSERT_NEXT_TOKEN(AnnotationNull, "@null");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationSuccess) {
    this->lex("@success");

    ASSERT_NEXT_TOKEN(AnnotationSuccess, "@success");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationGlobal) {
    this->lex("@global");

    ASSERT_NEXT_TOKEN(AnnotationGlobal, "@global");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationThread) {
    this->lex("@thread");

    ASSERT_NEXT_TOKEN(AnnotationThread, "@thread");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationIO) {
    this->lex("@io");

    ASSERT_NEXT_TOKEN(AnnotationIO, "@io");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationRegister) {
    this->lex("@register");

    ASSERT_NEXT_TOKEN(AnnotationRegister, "@register");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationMemory) {
    this->lex("@memory");

    ASSERT_NEXT_TOKEN(AnnotationMemory, "@memory");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationFlow) {
    this->lex("@flow");

    ASSERT_NEXT_TOKEN(AnnotationFlow, "@flow");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationAvailable) {
    this->lex("@available");

    ASSERT_NEXT_TOKEN(AnnotationAvailable, "@available");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationNoreturn) {
    this->lex("@noreturn");

    ASSERT_NEXT_TOKEN(AnnotationNoreturn, "@noreturn");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationCommon) {
    this->lex("@common");

    ASSERT_NEXT_TOKEN(AnnotationCommon, "@common");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationUncommon) {
    this->lex("@uncommon");

    ASSERT_NEXT_TOKEN(AnnotationUncommon, "@uncommon");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationOptimize) {
    this->lex("@optimize");

    ASSERT_NEXT_TOKEN(AnnotationOptimize, "@optimize");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationInline) {
    this->lex("@inline");

    ASSERT_NEXT_TOKEN(AnnotationInline, "@inline");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationPrefetch) {
    this->lex("@prefetch");

    ASSERT_NEXT_TOKEN(AnnotationPrefetch, "@prefetch");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationPure) {
    this->lex("@pure");

    ASSERT_NEXT_TOKEN(AnnotationPure, "@pure");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationThrows) {
    this->lex("@throws");

    ASSERT_NEXT_TOKEN(AnnotationThrows, "@throws");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationConst) {
    this->lex("@const");

    ASSERT_NEXT_TOKEN(AnnotationConst, "@const");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationRestrict) {
    this->lex("@restrict");

    ASSERT_NEXT_TOKEN(AnnotationRestrict, "@restrict");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationBrief) {
    this->lex("@brief");

    ASSERT_NEXT_TOKEN(AnnotationBrief, "@brief");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationSummary) {
    this->lex("@summary");

    ASSERT_NEXT_TOKEN(AnnotationSummary, "@summary");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationParam) {
    this->lex("@param");

    ASSERT_NEXT_TOKEN(AnnotationParam, "@param");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationReturn) {
    this->lex("@return");

    ASSERT_NEXT_TOKEN(AnnotationReturn, "@return");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationTodo) {
    this->lex("@todo");

    ASSERT_NEXT_TOKEN(AnnotationTodo, "@todo");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationSensitive) {
    this->lex("@sensitive");

    ASSERT_NEXT_TOKEN(AnnotationSensitive, "@sensitive");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AnnotationUntrusted) {
    this->lex("@untrusted");

    ASSERT_NEXT_TOKEN(AnnotationUntrusted, "@untrusted");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

// Combinations
TEST_F(LexerTest, IdentifierColonNumber) {
    this->lex("Int:32");

    ASSERT_NEXT_TOKEN(Identifier, "Int");
    ASSERT_NEXT_TOKEN(PunctuationColon, ":");
    ASSERT_NEXT_TOKEN(LiteralInteger, "32");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, IdentifierColonIdentifier) {
    this->lex("Char:ascii");

    ASSERT_NEXT_TOKEN(Identifier, "Char");
    ASSERT_NEXT_TOKEN(PunctuationColon, ":");
    ASSERT_NEXT_TOKEN(Identifier, "ascii");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, StarIdentifier) {
    this->lex("*Int");

    ASSERT_NEXT_TOKEN(OperatorStar, "*");
    ASSERT_NEXT_TOKEN(Identifier, "Int");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, DoubleStarIdentifier) {
    this->lex("**Int");

    ASSERT_NEXT_TOKEN(OperatorStar, "*");
    ASSERT_NEXT_TOKEN(OperatorStar, "*");
    ASSERT_NEXT_TOKEN(Identifier, "Int");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, SquareBracketsBeforeIdentifier) {
    this->lex("[5]Int");

    ASSERT_NEXT_TOKEN(PunctuationOpenBracket, "[");
    ASSERT_NEXT_TOKEN(LiteralInteger, "5");
    ASSERT_NEXT_TOKEN(PunctuationCloseBracket, "]");
    ASSERT_NEXT_TOKEN(Identifier, "Int");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, SquareBracketsAfterIdentifier) {
    this->lex("x[5]");

    ASSERT_NEXT_TOKEN(Identifier, "x");
    ASSERT_NEXT_TOKEN(PunctuationOpenBracket, "[");
    ASSERT_NEXT_TOKEN(LiteralInteger, "5");
    ASSERT_NEXT_TOKEN(PunctuationCloseBracket, "]");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AssignmentFollowedByUnaryOperator) {
    this->lex("a = &1");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(OperatorAssign, "=");
    ASSERT_NEXT_TOKEN(OperatorAmpersand, "&");
    ASSERT_NEXT_TOKEN(LiteralInteger, "1");
    ASSERT_NEXT_TOKEN(Undefined, "");
}

TEST_F(LexerTest, AssignmentToString) {
    this->lex("a = \"hello\"");

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(OperatorAssign, "=");
    ASSERT_NEXT_TOKEN(LiteralString, "\"hello\"");
}

TEST_F(LexerTest, WhitespaceAfterPeek) {
    this->lex("a cas b");

    ASSERT_EQ("a", this->lexer()->peek(1).str());
    ASSERT_EQ("cas", this->lexer()->peek(2).str());
    ASSERT_EQ("b", this->lexer()->peek(3).str());

    this->lexer()->setFilterWhitespace(false);

    ASSERT_NEXT_TOKEN(Identifier, "a");
    ASSERT_NEXT_TOKEN(Whitespace, " ");
    ASSERT_NEXT_TOKEN(OperatorCAS, "cas");
    ASSERT_NEXT_TOKEN(Whitespace, " ");
    ASSERT_NEXT_TOKEN(Identifier, "b");
    ASSERT_NEXT_TOKEN(Undefined, "");
}
