#include "SubtokenLexer.h"

#include <sstream>

#include <assert.h>

namespace Three {
    SubtokenLexer::SubtokenLexer(std::istream* stream) : _stream(stream), _subtoken(Subtoken::Type::Undefined) {
        _potentialDirective = true;
    }

    bool SubtokenLexer::characterPeek(char& c) {
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

    char SubtokenLexer::characterAdvance() {
        return _stream->get();
    }

    void SubtokenLexer::populateSubtokenBuffer() {
        // We need to populate _subtoken with an initial value
        if (_subtoken.type() == Subtoken::Type::Undefined) {
            _subtoken = this->advance();
        }
    }

    Subtoken SubtokenLexer::next() {
        this->populateSubtokenBuffer();

        Subtoken tmp = _subtoken;

        _subtoken = this->advance();

        return tmp;
    }

    Subtoken SubtokenLexer::peek() {
        this->populateSubtokenBuffer();

        return _subtoken;
    }

    Subtoken SubtokenLexer::advance() {
        std::stringstream s;
        char              c;
        Subtoken::Type    type = Subtoken::Type::EndOfInput;

        while (this->characterPeek(c)) {
            assert(c > 0);

            // std::cout << "subtoken: '" << c << "' : " << type << std::endl;

            // handle spans (comments, strings, whitespace, characters)
            switch (type) {
                case Subtoken::Type::Comment:
                    if (c == '\n') {
                        return Subtoken(type, s.str());
                    }

                    s << this->characterAdvance();
                    continue; // back to the while loop
                case Subtoken::Type::String:
                    s << this->characterAdvance();

                    if (c == '"') {
                        return Subtoken(type, s.str());
                    }

                    continue; // back to the while loop
                case Subtoken::Type::Character:
                    s << this->characterAdvance();

                    if (c == '\'') {
                        return Subtoken(type, s.str());
                    }

                    continue; // back to the while loop
                case Subtoken::Type::Whitespace:
                    if (c != ' ') {
                        return Subtoken(type, s.str());
                    }

                    s << this->characterAdvance();
                    continue;
                default:
                    break;
            }

            // handle the normal case
            switch (c) {
                case '\n':
                    if (type != Subtoken::Type::EndOfInput) {
                        return Subtoken(type, s.str());
                    }

                    this->characterAdvance();
                    _potentialDirective = true;

                    return Subtoken(Subtoken::Type::Newline, "\n");
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
                case '@':
                case '^':
                case '.':
                case '[':
                case ']':
                case '{':
                case '}':
                case '~':
                case '(':
                case ')':
                case ',':
                case ';':
                case ':':
                case '?':
                case '%':
                    if (type != Subtoken::Type::EndOfInput) {
                        return Subtoken(type, s.str());
                    }

                    s << this->characterAdvance();

                    if (c == '%' && _potentialDirective) {
                        _potentialDirective = false;
                        return this->lexDirective();
                    }

                    _potentialDirective = false;
                    return Subtoken(Subtoken::Type::Punctuation, s.str());
                case '#':
                    _potentialDirective = false;
                    if (type != Subtoken::Type::EndOfInput) {
                        return Subtoken(type, s.str());
                    }

                    type = Subtoken::Type::Comment;
                    s << this->characterAdvance();

                    break;
                case '"':
                    _potentialDirective = false;
                    if (type != Subtoken::Type::EndOfInput) {
                        return Subtoken(type, s.str());
                    }

                    type = Subtoken::Type::String;
                    s << this->characterAdvance();

                    break;
                case '\'':
                    _potentialDirective = false;
                    if (type != Subtoken::Type::EndOfInput) {
                        return Subtoken(type, s.str());
                    }

                    type = Subtoken::Type::Character;
                    s << this->characterAdvance();

                    break;
                case ' ':
                    _potentialDirective = false;
                    if (type != Subtoken::Type::EndOfInput) {
                        return Subtoken(type, s.str());
                    }

                    type = Subtoken::Type::Whitespace;
                    s << this->characterAdvance();

                    break;
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
                    _potentialDirective = false;
                    if (type == Subtoken::Type::EndOfInput) {
                        type = Subtoken::Type::Number;
                    }

                    if (type != Subtoken::Type::Number && type != Subtoken::Type::Word) {
                        return Subtoken(type, s.str());
                    }

                    s << this->characterAdvance();
                    break;
                default:
                    _potentialDirective = false;
                    type = Subtoken::Type::Word;
                    s << this->characterAdvance();
                    break;
            }
        }

        return Subtoken(type, s.str());
    }

    Subtoken SubtokenLexer::lexDirective() {
        std::stringstream s;
        char              c;

        while (this->characterPeek(c)) {
            if (c == '\n' || c == ' ') {
                break;
            }

            s << this->characterAdvance();
        }

        std::string str(s.str());

        if (str == "end") {
            return Subtoken(Subtoken::Type::DirectiveEnd, str);
        }

        if (str == "if") {
            return Subtoken(Subtoken::Type::DirectiveIf, str);
        }

        if (str == "else") {
            return Subtoken(Subtoken::Type::DirectiveElse, str);
        }

        if (str == "switch") {
            return Subtoken(Subtoken::Type::DirectiveSwitch, str);
        }

        if (str == "case") {
            return Subtoken(Subtoken::Type::DirectiveCase, str);
        }

        return Subtoken();
    }
}
