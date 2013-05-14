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
            String,
            Annotation,
            Comment,
            KeywordDef,
            KeywordEnd,
            KeywordReturn,
            KeywordIf,
            KeywordElse,
            KeywordImport
        } Type;

    public:
        Token(const std::string& string, const Type& type);
        Token();

        bool isEmpty();
        bool isEnd();

        bool isBinaryLiteralSubtoken() const;
        bool isHexLiteralSubtoken() const;

        std::string str() const;
        Type type() const;

    private:
        std::string _string;
        Type        _type;
    };
}
