#include "Token.h"

namespace Language {
    Token::Token(const std::string& string, const Type& type) :
            _string(string),
            _type(type) {
    }

    Token::Token() : _string(""), _type(Type::EndOfInput) {
    }

    bool Token::isEmpty() {
        return _string.empty();
    }

    bool Token::isEnd() {
        return this->isEmpty();
    }

    bool Token::isDataType() {
        std::string::const_iterator it;

        it = _string.begin();

        // if the first character is upper-case
        return (*it) >= 'A' && (*it) <= 'Z';
    }

    bool Token::isBinaryLiteralSubtoken() const {
        std::string::const_iterator it;

        it = _string.begin();

        if ((*it) != 'b') {
            return false;
        }

        for (++it; it < _string.end(); ++it) {
            if ((*it) != '0' && (*it) != '1') {
                return false;
            }
        }

        return true;
    }

    bool Token::isHexLiteralSubtoken() const {
        std::string::const_iterator it;

        it = _string.begin();

        if ((*it) != 'x') {
            return false;
        }

        for (++it; it < _string.end(); ++it) {
            if ((*it) >= '0' && (*it) <= '9') {
                break;
            }

            if ((*it) >= 'a' && (*it) <= 'f') {
                break;
            }

            if ((*it) >= 'A' && (*it) <= 'F') {
                break;
            }

            return false;
        }

        return true;
    }

    std::string Token::str() const {
        return _string;
    }

    Token::Type Token::type() const {
        return _type;
    }
}
