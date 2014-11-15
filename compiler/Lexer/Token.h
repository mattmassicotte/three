#pragma once

#include <string>

namespace Three {
    class Token {
    public:
        typedef enum {
            Undefined,
            EndOfInput,
            Newline,
            Comment,
            Whitespace,

            Identifier,

            PunctuationOpenBrace,
            PunctuationCloseBrace,
            PunctuationOpenParen,
            PunctuationCloseParen,
            PunctuationColon,
            PunctuationComma,
            PunctuationSemicolon,
            PunctuationOpenBracket,
            PunctuationCloseBracket,

            LiteralString,
            LiteralInteger,
            LiteralBinary,
            LiteralHex,
            LiteralReal,
            LiteralScientificNotation,
            LiteralTrue,
            LiteralFalse,
            LiteralNull,
            LiteralCharacter,

            OperatorScope,
            OperatorStar,
            OperatorAmpersand,
            OperatorMinus,
            OperatorIncrement,
            OperatorDecrement,
            OperatorPlus,
            OperatorDivide,
            OperatorMod,
            OperatorDot,
            OperatorArrow,

            OperatorEqual,
            OperatorDeepEqual,
            OperatorCompare,
            OperatorNotEqual,
            OperatorGreaterThan,
            OperatorLessThan,
            OperatorGreaterOrEqual,
            OperatorLessOrEqual,
            OperatorLogicalAnd,
            OperatorLogicalOr,
            OperatorLogicalXor,
            OperatorNot,

            OperatorBinaryNot,
            OperatorBinaryOr,
            OperatorBinaryXor,
            OperatorLeftShift,
            OperatorRightShift,

            OperatorAssign,
            OperatorAddAssign,
            OperatorSubtractAssign,
            OperatorMultiplyAssign,
            OperatorDivideAssign,
            OperatorModAssign,
            OperatorBitwiseAndAssign,
            OperatorBitwiseOrAssign,
            OperatorBitwiseXorAssign,
            OperatorLeftShiftAssign,
            OperatorRightShiftAssign,
            OperatorLogicalAndAssign,
            OperatorLogicalOrAssign,

            OperatorQuestionMark,
            OperatorCAS,

            MetafunctionSizeOf,
            MetafunctionCast,
            MetafunctionAlignof,
            MetafunctionOffsetof,
            MetafunctionTypeof,
            MetafunctionABIof,
            MetafunctionNameof,
            MetafunctionCardinalityof,
            MetafunctionNextarg,

            KeywordDef,
            KeywordEnd,
            KeywordReturn,
            KeywordEnsure,
            KeywordIf,
            KeywordElse,
            KeywordUnless,
            KeywordLoop,
            KeywordFor,
            KeywordBreak,
            KeywordContinue,
            KeywordImport,
            KeywordClosure,
            KeywordStructure,
            KeywordEnumeration,
            KeywordUnion,
            KeywordSwitch,
            KeywordCase,
            KeywordAtomic,
            KeywordBarrier,
            KeywordAbort,
            KeywordVararg,
            KeywordPublic,
            KeywordPrivate,
            KeywordNamespace,
            KeywordModule,
            KeywordInclude,
            KeywordLinkage,
            KeywordABI,
            KeywordAssert,
            KeywordDebug,

            AnnotationAccess,
            AnnotationVolatile,
            AnnotationAlias,
            AnnotationRead,
            AnnotationWrite,
            AnnotationAssert,
            AnnotationSentinel,
            AnnotationChecked,
            AnnotationNull,
            AnnotationSuccess,
            AnnotationGlobal,
            AnnotationThread,
            AnnotationIO,
            AnnotationRegister,
            AnnotationMemory,
            AnnotationFlow,
            AnnotationAvailable,
            AnnotationNoreturn,
            AnnotationCommon,
            AnnotationUncommon,
            AnnotationOptimize,
            AnnotationInline,
            AnnotationPrefetch,
            AnnotationSensitive,
            AnnotationUntrusted,

            AnnotationPure,
            AnnotationThrows,
            AnnotationConst,
            AnnotationRestrict,

            AnnotationBrief,
            AnnotationSummary,
            AnnotationParam,
            AnnotationReturn,
            AnnotationTodo
        } Type;

        const static uint32_t NonPrecedence     = 0;
        const static uint32_t MinimumPrecedence = 1;

    public:
        Token();
        Token(Token::Type type);
        Token(Token::Type type, const std::string& value);

        Token::Type type() const;
        std::string str() const;
        std::string strTrimmingFirstAndLast() const;
        std::string strWithWhitespace() const;

        uint32_t precedence() const;

        bool isEnd();
        bool isStatementEnding();

        bool isAnnotation() const;
        bool isTypeAnnotation() const;
        bool isOperator() const;
        bool isUnaryOperator() const;
        bool isTernaryOperator() const;
        bool isLeftAssociative() const;

        bool isOpeningSpan() const;
        Token::Type closingCounterpart() const;
        bool mightBeClosedByEndKeyword() const;

    private:
        Token::Type _type;
        std::string _value;
        std::string _fullValue;
    };
}
