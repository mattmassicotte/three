#pragma once

#include <string>

namespace Language {
    class Token {

    public:
        typedef enum {
            EndOfInput,
            Newline,
            Identifier,
            NumericLiteral,
            TrueLiteral,
            FalseLiteral,
            NullLiteral,
            Punctuation,
            PunctuationOpenBrace,
            PunctuationCloseBrace,
            PunctuationOpenParen,
            PunctuationCloseParen,
            PunctuationColon,
            PunctuationSemicolon,
            String,
            Annotation,
            Comment,
            KeywordDef,
            KeywordEnd,
            KeywordReturn,
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
            KeywordSwitch,
            KeywordCase,
            KeywordAtomic,
            KeywordBarrier,
            KeywordAbort,
            KeywordSizeof,
            KeywordAlignof,
            KeywordTypeof,
            KeywordOffsetof,
            KeywordVararg,
            KeywordVarargNext,
            KeywordLinkage,
            KeywordInclude,
            KeywordPublic,
            KeywordAbi,
            KeywordDebug,
            KeywordVersion,
            KeywordNamespace,
            KeywordModule,
            Operator
        } Type;

        const static uint32_t NonPrecedence     = 0;
        const static uint32_t MinimumPrecedence = 1;

    public:
        Token(const std::string& string, const Type& type);
        Token();

        bool isEmpty();
        bool isEnd();
        bool isStatementEnding();

        bool isBinaryLiteralSubtoken() const;
        bool isHexLiteralSubtoken() const;

        std::string str() const;
        Type type() const;

        uint32_t precedence() const;

    private:
        std::string _string;
        Type        _type;
    };
}
