#pragma once

#include <string>

namespace Three {
    class Subtoken {
    public:
        typedef enum {
            Undefined,
            EndOfInput,
            Newline,
            Word,
            Number,
            String,
            Character,
            Punctuation,
            Comment,
            Whitespace,

            DirectiveSwitch,
            DirectiveCase,
            DirectiveElse,
            DirectiveEnd,
            DirectiveIf
        } Type;

    public:
        Subtoken();
        Subtoken(Subtoken::Type type);
        Subtoken(Subtoken::Type type, const std::string& value);

        Subtoken::Type type() const;
        std::string str() const;

    private:
        Subtoken::Type _type;
        std::string _value;
    };
}
