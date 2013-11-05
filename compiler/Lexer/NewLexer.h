#pragma once

#include "Token.h"
#include "SubtokenLexer.h"

#include <vector>
#include <map>

namespace Three {
    class NewLexer {
    public:
        NewLexer(std::istream* stream);
        virtual ~NewLexer();

    protected:
        // working with the input stream
        Subtoken subtokenPeek();
        Subtoken subtokenAdvance();
        bool subtokenAdvanceIfEqual(const std::string& value);

    public:
        // working with subtoken stream
        Token next();
        bool nextIf(const std::string& value);
        bool nextIf(Token::Type type);
        Token peek(unsigned int distance = 1);
        bool atEnd();

        bool filterWhitespace() const;
        void setFilterWhitespace(bool value);

    protected:
        Token advance();

        Token lexWord();
        Token lexNumber();
        Token lexPunctuation();
        Token lexAnnotation();

    private:
        SubtokenLexer* _lexer;
        std::vector<Token> _tokenBuffer;

        std::map<std::string, Token::Type> _keywordMap;
        std::map<std::string, Token::Type> _annotationMap;

        bool _filterWhitespace;
    };
}
