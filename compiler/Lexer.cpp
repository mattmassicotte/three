#include "Lexer.h"

#include <string>
#include <sstream>

#include <assert.h>

namespace Language {
    Lexer::Lexer(std::istream* stream) : _stream(stream) {
        _stream = stream;
        _subtoken = this->advanceSubtoken();

        _keywordMap["import"]    = Token::Type::KeywordImport;
        _keywordMap["def"]       = Token::Type::KeywordDef;
        _keywordMap["end"]       = Token::Type::KeywordEnd;
        _keywordMap["return"]    = Token::Type::KeywordReturn;
        _keywordMap["if"]        = Token::Type::KeywordIf;
        _keywordMap["else"]      = Token::Type::KeywordElse;
        _keywordMap["unless"]    = Token::Type::KeywordUnless;
        _keywordMap["loop"]      = Token::Type::KeywordLoop;
        _keywordMap["for"]       = Token::Type::KeywordFor;
        _keywordMap["continue"]  = Token::Type::KeywordContinue;
        _keywordMap["break"]     = Token::Type::KeywordBreak;
        _keywordMap["true"]      = Token::Type::TrueLiteral;
        _keywordMap["false"]     = Token::Type::FalseLiteral;
        _keywordMap["null"]      = Token::Type::NullLiteral;
        _keywordMap["do"]        = Token::Type::KeywordClosure;
        _keywordMap["struct"]    = Token::Type::KeywordStructure;
        _keywordMap["enum"]      = Token::Type::KeywordEnumeration;
        _keywordMap["switch"]    = Token::Type::KeywordSwitch;
        _keywordMap["case"]      = Token::Type::KeywordCase;
        _keywordMap["atomic"]    = Token::Type::KeywordAtomic;
        _keywordMap["barrier"]   = Token::Type::KeywordBarrier;
        _keywordMap["abort"]     = Token::Type::KeywordAbort;
        _keywordMap["cas"]       = Token::Type::Operator;
        _keywordMap["and"]       = Token::Type::Operator;
        _keywordMap["or"]        = Token::Type::Operator;
        _keywordMap["sizeof"]    = Token::Type::KeywordSizeof;
        _keywordMap["alignof"]   = Token::Type::KeywordAlignof;
        _keywordMap["typeof"]    = Token::Type::KeywordTypeof;
        _keywordMap["offsetof"]  = Token::Type::KeywordOffsetof;
        _keywordMap["Vararg"]    = Token::Type::KeywordVararg;
        _keywordMap["nextarg"]   = Token::Type::KeywordVarargNext;
        _keywordMap["linkage"]   = Token::Type::KeywordLinkage;
        _keywordMap["include"]   = Token::Type::KeywordInclude;
        _keywordMap["public"]    = Token::Type::KeywordPublic;
        _keywordMap["abi"]       = Token::Type::KeywordAbi;
        _keywordMap["debug"]     = Token::Type::KeywordDebug;
        _keywordMap["version"]   = Token::Type::KeywordVersion;
        _keywordMap["namespace"] = Token::Type::KeywordNamespace;
        _keywordMap["module"]    = Token::Type::KeywordModule;
    }

    bool Lexer::characterPeek(char& c) {
        if (_stream->eof()) {
            return false;
        }

        char tmp = _stream->peek();

        if (tmp == -1) {
            return false;
        }

        c = tmp;

        return true;
    }

    char Lexer::characterAdvance() {
        return _stream->get();
    }

    void Lexer::translateKeyword(Token& token) {
        std::string str = token.str();

        if (_keywordMap.find(str) != _keywordMap.end()) {
            token = Token(str, _keywordMap[str]);
        }
    }

    Token Lexer::punctuationToken(const char c) {
        Token::Type type;

        switch (c) {
            case '{':
                type = Token::Type::PunctuationOpenBrace;
                break;
            case '}':
                type = Token::Type::PunctuationCloseBrace;
                break;
            case '(':
                type = Token::Type::PunctuationOpenParen;
                break;
            case ')':
                type = Token::Type::PunctuationCloseParen;
                break;
            case ':':
                type = Token::Type::PunctuationColon;
                break;
            case ';':
                type = Token::Type::PunctuationSemicolon;
                break;
            default:
                type = Token::Type::Punctuation;
                break;
        }

        return Token(std::string(1, c), type);
    }

    Token Lexer::nextSubtoken() {
        Token tmp = _subtoken;

        _subtoken = this->advanceSubtoken();

        return tmp;
    }

    Token Lexer::peekSubtoken() {
        return _subtoken;
    }

    Token Lexer::advanceSubtoken() {
        std::stringstream s;
        char              c;
        Token::Type       type = Token::Type::EndOfInput;

        while (this->characterPeek(c)) {
            assert(c > 0);

            // std::cout << "lex: '" << c << "'" << std::endl;
            switch (type) {
                case Token::Type::String:
                    if (c == '"') {
                        this->characterAdvance();
                        return Token(s.str(), type);
                    }

                    s << this->characterAdvance();
                    continue;
                default:
                    break;
            }

            switch (c) {
                case ' ':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    this->characterAdvance();
                    break;
                case '\n':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    // advance past the newline
                    this->characterAdvance();

                    return Token("", Token::Type::Newline);
                case '"':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }
                    
                    type = Token::Type::String;
                    
                    this->characterAdvance(); // skip past the opening quote
                    break;
                case '/':
                case '*':
                case '-':
                case '+':
                case '!':
                case '=':
                case '|':
                case '>':
                case '<':
                case '&':
                case '%':
                case '^':
                case '.':
                case '~':
                case '?':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    s << this->characterAdvance();

                    return Token(s.str(), Token::Type::Operator);
                case '{':
                case '}':
                case '(':
                case ')':
                case ':':
                case '@':
                case '\'':
                case ';':
                case ',':
                case '\\':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    this->characterAdvance();

                    return this->punctuationToken(c);
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                    if (type == Token::Type::Identifier) {
                        s << this->characterAdvance();
                        break;
                    }

                    if ((type == Token::Type::EndOfInput) || (type == Token::Type::NumericLiteral)) {
                        type = Token::Type::NumericLiteral;
                        s << this->characterAdvance();
                        break;
                    }

                    return Token(s.str(), type);
                default:
                    if ((type != Token::Type::EndOfInput) && (type != Token::Type::Identifier)) {
                        return Token(s.str(), type);
                    }

                    type = Token::Type::Identifier;
                    s << this->characterAdvance();
                    break;
            }
        }

        return Token(s.str(), type);
    }

    Token Lexer::lexNumericLiteral() {
        std::stringstream s;
        Token             t;

        assert(this->peekSubtoken().type() == Token::Type::NumericLiteral);
        t = this->nextSubtoken();
        s << t.str();

        if (t.str() == "0") {
            t = this->peekSubtoken();
            if (t.isBinaryLiteralSubtoken() || t.isHexLiteralSubtoken()) {
                s << this->nextSubtoken().str();
            }

            return Token(s.str(), Token::Type::NumericLiteral);
        }

        t = this->peekSubtoken();
        if (t.str() == ".") {
            s << this->nextSubtoken().str();

            assert(this->peekSubtoken().type() == Token::Type::NumericLiteral);

            s << this->nextSubtoken().str();

            t = this->peekSubtoken();
        }

        if (t.str() == "e") {
            s << this->nextSubtoken().str();

            if (this->peekSubtoken().str() == "-") {
                s << this->nextSubtoken().str();
            }

            if (this->peekSubtoken().type() == Token::Type::NumericLiteral) {
                s << this->nextSubtoken().str();
            }
        }

        return Token(s.str(), Token::Type::NumericLiteral);
    }

    Token Lexer::lexAnnotation() {
        std::stringstream s;

        assert(this->peekSubtoken().str().at(0) == '@');

        s << this->nextSubtoken().str();

        assert(this->peekSubtoken().type() == Token::Type::Identifier);

        s << this->nextSubtoken().str();

        return Token(s.str(), Token::Type::Annotation);
    }

    Token Lexer::lexPunctuation() {
        switch (this->peekSubtoken().str().at(0)) {
            case '@':
                return this->lexAnnotation();
        }

        return this->nextSubtoken();
    }

    Token Lexer::lexIdentifier() {
        Token t = this->nextSubtoken();

        // if this is a keyword, translate it
        this->translateKeyword(t);

        return t;
    }

    Token Lexer::lexOperator() {
        std::stringstream s;

        s << this->nextSubtoken().str();

        // easy case, one char and the next is not an operator
        if (this->peekSubtoken().type() != Token::Type::Operator) {
            return Token(s.str(), Token::Type::Operator);
        }

        // ternary operators do not support compounding
        if (s.str() == "cas" || s.str() == "?") {
            return Token(s.str(), Token::Type::Operator);
        }

        char c = s.str().at(0);

        // deal with comments
        if (c == '/' && this->peekSubtoken().str().at(0) == '/') {
            do {
                s << this->nextSubtoken().str();
            } while (this->peekSubtoken().type() != Token::Type::Newline);

            // don't actually return a comment token
            return this->nextSubtoken();
        }

        // possibly allow a double operator
        if (this->peekSubtoken().str().at(0) == c) {
            switch (c) {
                case '/':
                case '*':
                    // these characters cannot be doubled
                    break;
                case '+':
                case '-':
                    s << this->nextSubtoken().str();

                    // these two do not support any further compounding
                    return Token(s.str(), Token::Type::Operator);
                case '=':
                case '<':
                case '>':
                case '&':
                case '|':
                case '^':
                    s << this->nextSubtoken().str();
                    break;
                default:
                    assert(0 && "Logic error lexing operators");
                    break;
            }
        }

        // take care of compound assignment
        if (this->peekSubtoken().str().at(0) == '=') {
            s << this->nextSubtoken().str();
        }

        // the deep-equality and comparison operators
        switch (this->peekSubtoken().str().at(0)) {
            case '=':
                if (s.str() == "==") {
                    s << this->nextSubtoken().str();
                }
                break;
            case '>':
                if (s.str() == "<=") {
                    s << this->nextSubtoken().str();
                }
                break;
            default:
                break;
        }

        return Token(s.str(), Token::Type::Operator);
    }

    Token Lexer::nextToken() {
        Token t = this->peekSubtoken();

        switch (t.type()) {
            case Token::Type::NumericLiteral:
                return this->lexNumericLiteral();
            case Token::Type::Punctuation:
                return this->lexPunctuation();
            case Token::Type::Identifier:
                return this->lexIdentifier();
            case Token::Type::Operator:
                return this->lexOperator();
            case Token::Type::EndOfInput:
            case Token::Type::String:
            default:
                break;
        }

        return this->nextSubtoken();
    }

    Token Lexer::next() {
        Token t;

        _tokenBuffer.push_back(this->nextToken());

        t = _tokenBuffer.front();

        // remove the first token, which is now out of date
        _tokenBuffer.erase(_tokenBuffer.begin());

        return t;
    }

    bool Lexer::nextIf(const std::string& value) {
        if (this->peek().str() == value) {
            this->next();
            return true;
        }

        return false;
    }

    Token Lexer::peek(unsigned int distance) {
        assert(distance > 0);

        for (int i = (distance - _tokenBuffer.size()); i > 0; --i) {
            _tokenBuffer.push_back(this->nextToken());
        }

        return _tokenBuffer.at(distance - 1);
    }
}
