#include "Lexer.h"

#include <string>
#include <sstream>

#include <assert.h>

namespace Language {
    Lexer::Lexer(std::istream* stream) : _stream(stream) {
        _stream = stream;
        _subtoken = this->advanceSubtoken();

        _keywordMap["import"]  = Token::Type::KeywordImport;
        _keywordMap["def"]     = Token::Type::KeywordDef;
        _keywordMap["end"]     = Token::Type::KeywordEnd;
        _keywordMap["return"]  = Token::Type::KeywordReturn;
        _keywordMap["if"]      = Token::Type::KeywordIf;
        _keywordMap["else"]    = Token::Type::KeywordElse;
        _keywordMap["true"]    = Token::Type::TrueLiteral;
        _keywordMap["false"]   = Token::Type::FalseLiteral;
        _keywordMap["null"]    = Token::Type::NullLiteral;
        _keywordMap["do"]      = Token::Type::KeywordClosure;
        _keywordMap["struct"]  = Token::Type::KeywordStructure;
        _keywordMap["enum"]    = Token::Type::KeywordEnumeration;
        _keywordMap["switch"]  = Token::Type::KeywordSwitch;
        _keywordMap["case"]    = Token::Type::KeywordCase;
        _keywordMap["atomic"]  = Token::Type::KeywordAtomic;
        _keywordMap["barrier"] = Token::Type::KeywordBarrier;
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
                case Token::Type::Comment:
                    if (c == '\n') {
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
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    s << this->characterAdvance();

                    this->characterPeek(c);
                    if (c == '/') {
                        type = Token::Type::Comment;
                    }
                    break;
                case '*':
                case '-':
                case '+':
                case '!':
                case '=':
                case '>':
                case '<':
                case '.':
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

    Token Lexer::lexForwardSlash() {
        std::stringstream s;

        assert(this->peekSubtoken().str().at(0) == '/');

        s << this->nextSubtoken().str();

        if (this->peekSubtoken().str().at(0) != '/') {
            return Token(s.str(), Token::Type::Punctuation);
        }

        do {
            s << this->nextSubtoken().str();
        } while (this->peekSubtoken().type() != Token::Type::Newline);

        return Token(s.str(), Token::Type::Comment);
    }

    Token Lexer::lexPunctuation() {
        switch (this->peekSubtoken().str().at(0)) {
            case '@':
                return this->lexAnnotation();
            case '/':
                return this->lexForwardSlash();
        }

        return this->nextSubtoken();
    }

    Token Lexer::lexIdentifier() {
        Token t = this->nextSubtoken();

        // if this is a keyword, translate it
        this->translateKeyword(t);

        return t;
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
