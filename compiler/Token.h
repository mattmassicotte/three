#pragma once

#include <string>

namespace Language {
    class Token {

    public:
        typedef enum {
            EndOfInput,
            Newline,
            DataType,
            Identifier,
            NumericLiteral,
            Punctuation,
            String,
            StringEscape,
            Annotation,
            Comment,
            KeywordDef,
            KeywordEnd
        } Type;

    public:
        Token(const std::string& string, const Type& type);
        Token();

        bool isEmpty();
        bool isEnd();
        bool isDataType();

        bool isBinaryLiteralSubtoken() const;
        bool isHexLiteralSubtoken() const;

        std::string str() const;
        Type type() const;

    private:
        std::string _string;
        Type        _type;
    };
}
