#include "Lexer.h"

#include <string>
#include <sstream>

#include <assert.h>

namespace Language {
    Lexer::Lexer(std::istream* stream) : _stream(stream) {
        _stream = stream;
        _subtoken = this->advanceSubtoken();
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

            switch (c) {
                case ' ':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    // advance past the space
                    this->characterAdvance();
                    break;
                case '\n':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    // advance past the newline
                    this->characterAdvance();

                    return Token("<newline>", Token::Type::Newline);
                case '.':
                case '"':
                case '{':
                case '}':
                case '(':
                case ')':
                case ':':
                case '*':
                case '-':
                case '+':
                case '!':
                case '=':
                case '@':
                case '>':
                case '<':
                case '\'':
                case ';':
                case ',':
                case '\\':
                    if (type != Token::Type::EndOfInput) {
                        return Token(s.str(), type);
                    }

                    s << this->characterAdvance();

                    return Token(s.str(), Token::Type::Punctuation);
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
                    if (type == Token::Type::NumericLiteral) {
                        return Token(s.str(), type);
                    }

                    type = Token::Type::Identifier;
                    s << this->characterAdvance();
                    break;
            }
        }
        
        return Token("", Token::Type::EndOfInput);
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
            case '"':
                return this->lexString();
        }

        return this->nextSubtoken();
    }

    Token Lexer::lexString() {
        std::stringstream s;

        assert(this->peekSubtoken().str().at(0) == '"');

        do {
            s << this->nextSubtoken().str();
        } while (this->peekSubtoken().str().at(0) != '"');

        s << this->nextSubtoken().str();

        return Token(s.str(), Token::Type::String);
    }

    Token Lexer::nextToken() {
        std::stringstream s;
        Token::Type       type;

        while (1) {
            Token t = this->peekSubtoken();

            switch (t.type()) {
                case Token::Type::NumericLiteral:
                    return this->lexNumericLiteral();
                case Token::Type::Punctuation:
                    return this->lexPunctuation();
                case Token::Type::Identifier:
                case Token::Type::EndOfInput:
                case Token::Type::String:
                default:
                    break;
            }

            return this->nextSubtoken();
        }

        return Token(s.str(), Token::Type::Identifier);
    }
}
