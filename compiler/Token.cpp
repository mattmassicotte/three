#include "Token.h"

namespace Three {
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

    bool Token::isStatementEnding() {
        return this->isEnd() || (_type == Type::Newline);
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

    bool Token::isUnaryOperator() const {
        if (this->type() != Operator) {
            return false;
        }

        if (_string.length() != 1) {
            return false;
        }

        switch (_string.at(0)) {
            case '*':
            case '&':
            case '-':
                return true;
        }

        return false;
    }

    bool Token::isTernaryOperator() const {
        if (this->type() != Operator) {
            return false;
        }

        return _string == "?" || _string == "cas";
    }

    bool Token::isLeftAssociative() const {
        if (_string == "+") return true;
        if (_string == "*") return true;
        if (_string == "-") return true;
        if (_string == "<") return true;
        if (_string == "<=") return true;
        if (_string == ">") return true;
        if (_string == ">=") return true;
        
        if (_string == "==") return true;
        if (_string == "!=") return true;

        return false;
    }

    std::string Token::str() const {
        return _string;
    }

    Token::Type Token::type() const {
        return _type;
    }

    uint32_t Token::precedence() const {
        if (_string == "[") return 16;
        if (_string == "++") return 16; // suffix
        if (_string == "--") return 16; // suffix
        if (_string == ".") return 16;
        if (_string == "->") return 16;
        
        if (_string == "*") return 15;
        if (_string == "%") return 15;
        if (_string == "/") return 15;
        
        if (_string == "+") return 14;
        if (_string == "-") return 14;
        
        if (_string == "<<") return 13;
        if (_string == ">>") return 13;
        
        if (_string == "<") return 12;
        if (_string == "<=") return 12;
        if (_string == ">") return 12;
        if (_string == ">=") return 12;
        
        if (_string == "==") return 11;
        if (_string == "!=") return 11;
        
        if (_string == "&") return 10;
        
        if (_string == "|") return 9;
        
        if (_string == "&&") return 8;
        
        if (_string == "||") return 7;
        
        if (_string == "?") return 6;
        if (_string == "cas") return 6;
        
        if (_string == "=") return 5;
        if (_string == "+=") return 5;
        if (_string == "-=") return 5;
        if (_string == "*=") return 5;
        if (_string == "/=") return 5;
        if (_string == "||=") return 5;
        if (_string == "&&=") return 5;

        return Token::NonPrecedence;
    }
}
