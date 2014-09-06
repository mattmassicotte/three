#include "ParseHelper.h"
#include "compiler/Lexer/Lexer.h"

namespace Three {
    ParseHelper::ParseHelper(Lexer* lexer) : _lexer(lexer) {
    }

    ParseHelper::~ParseHelper() {
    }

    Token ParseHelper::peek(unsigned int distance) {
        return _lexer->peek(distance);
    }

    Lexer* ParseHelper::lexer() const {
        return _lexer;
    }

    Token ParseHelper::next() {
        return _lexer->next();
    }

    std::string ParseHelper::nextStr() {
        return this->next().str();
    }

    bool ParseHelper::peekCompare(const std::string& value) {
        return this->peek().str() == value;
    }

    bool ParseHelper::peekCompare(const Token::Type& type) {
        return this->peek().type() == type;
    }

    bool ParseHelper::nextIf(const std::string& value) {
        return _lexer->nextIf(value);
    }

    bool ParseHelper::nextIf(const Token::Type& type) {
        return _lexer->nextIf(type);
    }

    bool ParseHelper::peekUntil(unsigned int* distance, std::function<bool (const Token& token)> func) {
        assert(distance);

        do {
            if (!func(this->peek(*distance))) {
                break;
            }

            *distance += 1;
        } while (true);

        return true;
    }

    bool ParseHelper::parseUntil(bool advanceOnStop, std::function<bool (const Token& token)> func) {
        do {
            Token t = this->peek();

            if (t.type() == Token::Type::EndOfInput) {
                return false;
            }

            if (func(t)) {
                break;
            }

            // TODO: this is an infinite loop if func does not call next at least once.
        } while (1);

        if (advanceOnStop) {
            this->next();
        }

        return true;
    }

    bool ParseHelper::parseUntilEnd(std::function<bool (void)> func) {
        for (int i = 0; i < 100000; ++i) {
            // skip past newlines
            this->parseNewlines();

            Token t = this->peek();
            switch (t.type()) {
                case Token::Type::EndOfInput:
                case Token::Type::Undefined:
                    return false; // this is a failure
                case Token::Type::KeywordEnd:
                    this->next();
                    return true;
                default:
                    if (func()) {
                        return true;
                    }
                    break;
            }
        }

        assert(0 && "Exceeded the maximum number of attempts in parseUntilEnd");

        return false;
    }

    bool ParseHelper::parseNewline() {
        return this->nextIf(Token::Type::Newline);
    }

    bool ParseHelper::parseNewlines() {
        if (!this->parseNewline()) {
            return false;
        }

        while (this->parseNewline()) {
            ;
        }

        return true;
    }
}
