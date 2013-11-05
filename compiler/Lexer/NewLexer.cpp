#include "NewLexer.h"

#include <assert.h>

namespace Three {
    NewLexer::NewLexer(std::istream* stream) {
        _lexer = new SubtokenLexer(stream);

        _keywordMap["true"] = Token::Type::LiteralTrue;
        _keywordMap["false"] = Token::Type::LiteralFalse;
        _keywordMap["null"] = Token::Type::LiteralNull;

        _keywordMap["cas"] = Token::Type::OperatorCAS;

        _keywordMap["sizeof"] = Token::Type::MetafunctionSizeOf;
        _keywordMap["cardinalityof"] = Token::Type::MetafunctionCardinalityof;
        _keywordMap["alignof"] = Token::Type::MetafunctionAlignof;
        _keywordMap["offsetof"] = Token::Type::MetafunctionOffsetof;
        _keywordMap["typeof"] = Token::Type::MetafunctionTypeof;
        _keywordMap["nameof"] = Token::Type::MetafunctionNameof;

        _keywordMap["def"] = Token::Type::KeywordDef;
        _keywordMap["end"] = Token::Type::KeywordEnd;
        _keywordMap["return"] = Token::Type::KeywordReturn;
        _keywordMap["ensure"] = Token::Type::KeywordEnsure;
        _keywordMap["if"] = Token::Type::KeywordIf;
        _keywordMap["else"] = Token::Type::KeywordElse;
        _keywordMap["unless"] = Token::Type::KeywordUnless;
        _keywordMap["loop"] = Token::Type::KeywordLoop;
        _keywordMap["for"] = Token::Type::KeywordFor;
        _keywordMap["break"] = Token::Type::KeywordBreak;
        _keywordMap["continue"] = Token::Type::KeywordContinue;
        _keywordMap["import"] = Token::Type::KeywordImport;
        _keywordMap["do"] = Token::Type::KeywordClosure;
        _keywordMap["struct"] = Token::Type::KeywordStructure;
        _keywordMap["enum"] = Token::Type::KeywordEnumeration;
        _keywordMap["switch"] = Token::Type::KeywordSwitch;
        _keywordMap["case"] = Token::Type::KeywordCase;
        _keywordMap["atomic"] = Token::Type::KeywordAtomic;
        _keywordMap["barrier"] = Token::Type::KeywordBarrier;
        _keywordMap["abort"] = Token::Type::KeywordAbort;
        _keywordMap["Vararg"] = Token::Type::KeywordVararg;
        _keywordMap["nextarg"] = Token::Type::KeywordVarargNext;
        _keywordMap["public"] = Token::Type::KeywordPublic;
        _keywordMap["private"] = Token::Type::KeywordPrivate;
        _keywordMap["namespace"] = Token::Type::KeywordNamespace;
        _keywordMap["module"] = Token::Type::KeywordModule;
        _keywordMap["include"] = Token::Type::KeywordInclude;
        _keywordMap["linkage"] = Token::Type::KeywordLinkage;
        _keywordMap["abi"] = Token::Type::KeywordABI;
        _keywordMap["assert"] = Token::Type::KeywordAssert;

        _annotationMap["read"] = Token::Type::AnnotationRead;
        _annotationMap["write"] = Token::Type::AnnotationWrite;
        _annotationMap["assert"] = Token::Type::AnnotationAssert;
        _annotationMap["sentinel"] = Token::Type::AnnotationSentinel;
        _annotationMap["checked"] = Token::Type::AnnotationChecked;
        _annotationMap["null"] = Token::Type::AnnotationNull;
        _annotationMap["success"] = Token::Type::AnnotationSuccess;
        _annotationMap["global"] = Token::Type::AnnotationGlobal;
        _annotationMap["thread"] = Token::Type::AnnotationThread;
        _annotationMap["io"] = Token::Type::AnnotationIO;
        _annotationMap["flow"] = Token::Type::AnnotationFlow;
        _annotationMap["available"] = Token::Type::AnnotationAvailable;
        _annotationMap["noreturn"] = Token::Type::AnnotationNoreturn;
        _annotationMap["common"] = Token::Type::AnnotationCommon;
        _annotationMap["uncommon"] = Token::Type::AnnotationUncommon;
        _annotationMap["optimize"] = Token::Type::AnnotationOptimize;
        _annotationMap["inline"] = Token::Type::AnnotationInline;
        _annotationMap["prefetch"] = Token::Type::AnnotationPrefetch;
        _annotationMap["memory"] = Token::Type::AnnotationMemory;
        _annotationMap["pure"] = Token::Type::AnnotationPure;
        _annotationMap["throws"] = Token::Type::AnnotationThrows;
        _annotationMap["released"] = Token::Type::AnnotationReleased;

        _filterWhitespace = true;
    }

    NewLexer::~NewLexer() {
        delete _lexer;
    }

    Subtoken NewLexer::subtokenPeek() {
        return _lexer->peek();
    }

    Subtoken NewLexer::subtokenAdvance() {
        return _lexer->next();
    }

    bool NewLexer::subtokenAdvanceIfEqual(const std::string& value) {
        if (this->subtokenPeek().str() == value) {
            this->subtokenAdvance();
            return true;
        }

        return false;
    }

    Token NewLexer::next() {
        Token tmp;

        _tokenBuffer.push_back(this->advance());

        tmp = _tokenBuffer.front();

        // remove the first token, which is now out of date
        _tokenBuffer.erase(_tokenBuffer.begin());

        return tmp;
    }

    bool NewLexer::nextIf(const std::string& value) {
        if (this->peek().str() == value) {
            this->next();
            return true;
        }

        return false;
    }

    bool NewLexer::nextIf(Token::Type type) {
        if (this->peek().type() == type) {
            this->next();
            return true;
        }

        return false;
    }

    Token NewLexer::peek(unsigned int distance) {
        assert(distance > 0);

        for (int i = (distance - _tokenBuffer.size()); i > 0; --i) {
            _tokenBuffer.push_back(this->advance());
        }

        return _tokenBuffer.at(distance - 1);
    }

    bool NewLexer::atEnd() {
        Token t = this->peek();

        return t.type() == Token::Type::EndOfInput || t.type() == Token::Type::Undefined;
    }

    bool NewLexer::filterWhitespace() const {
        return _filterWhitespace;
    }

    void NewLexer::setFilterWhitespace(bool value) {
        _filterWhitespace = value;
    }

    Token NewLexer::advance() {
        Subtoken t = this->subtokenPeek();

        // Move past whitespace, if needed
        while (_filterWhitespace && t.type() == Subtoken::Type::Whitespace) {
            this->subtokenAdvance();
            t = this->subtokenPeek();
        }

        // filter out comments
        while (t.type() == Subtoken::Type::Comment) {
            this->subtokenAdvance();
            t = this->subtokenPeek();
        }

        switch (t.type()) {
            case Subtoken::Type::Word:
                return this->lexWord();
            case Subtoken::Type::Number:
                return this->lexNumber();
            case Subtoken::Type::String:
                this->subtokenAdvance();
                // the "str" property includes the quotes
                return Token(Token::Type::LiteralString, t.stringValue());
            case Subtoken::Type::Punctuation:
                if (t.str()[0] == '@') {
                    return this->lexAnnotation();
                }

                return this->lexPunctuation();
            case Subtoken::Type::Newline:
                this->subtokenAdvance();
                return Token(Token::Type::Newline, "\n");
            case Subtoken::Type::Whitespace:
                this->subtokenAdvance();
                return Token(Token::Type::Whitespace, t.str());
            default:
                break;
        }

        return Token();
    }

    Token NewLexer::lexWord() {
        Token::Type type = Token::Type::Identifier;
        Subtoken t = this->subtokenAdvance();

        // we have either a 0x or 0b
        if (t.str()[0] == '0') {
            switch (t.str()[1]) {
                case 'b':
                    return Token(Token::Type::LiteralBinary, t.str());
                case 'x':
                    return Token(Token::Type::LiteralHex, t.str());
                default:
                    break;
            }
        }

        if (_keywordMap.find(t.str()) != _keywordMap.end()) {
            type = _keywordMap[t.str()];
        }

        return Token(type, t.str());
    }

    Token NewLexer::lexNumber() {
        std::stringstream s;

        s << this->subtokenAdvance().str();

        if (this->subtokenPeek().str() == ".") {
            s << this->subtokenAdvance().str();

            if (this->subtokenPeek().type() != Subtoken::Type::Number) {
                return Token();
            }

            s << this->subtokenAdvance().str();

            return Token(Token::Type::LiteralReal, s.str());
        }

        return Token(Token::Type::LiteralInteger, s.str());
    }

    Token NewLexer::lexPunctuation() {
        Subtoken t = this->subtokenAdvance();

        // simple cases, that have no compounding
        switch (t.str()[0]) {
            case ';':
                return Token(Token::Type::PunctuationSemicolon, ";");
            case ',':
                return Token(Token::Type::PunctuationComma, ",");
            case '[':
                return Token(Token::Type::PunctuationOpenBracket, "[");
            case ']':
                return Token(Token::Type::PunctuationCloseBracket, "]");
            case '{':
                return Token(Token::Type::PunctuationOpenBrace, "{");
            case '}':
                return Token(Token::Type::PunctuationCloseBrace, "}");
            case '(':
                return Token(Token::Type::PunctuationOpenParen, "(");
            case ')':
                return Token(Token::Type::PunctuationCloseParen, ")");
            case '~':
                return Token(Token::Type::OperatorBinaryNot, "~");
            case '?':
                return Token(Token::Type::OperatorQuestionMark, "?");
            case '.':
                return Token(Token::Type::OperatorDot, ".");
            default:
                break;
        }

        // cases with compounding
        switch (t.str()[0]) {
            case ':':
                if (this->subtokenPeek().str() == ":") {
                    this->subtokenAdvance();

                    return Token(Token::Type::OperatorScope, "::");
                }

                return Token(Token::Type::PunctuationColon, ":");
            case '-':
                if (this->subtokenAdvanceIfEqual("-")) {
                    return Token(Token::Type::OperatorDecrement, "--");
                }

                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorSubtractAssign, "-=");
                }

                if (this->subtokenAdvanceIfEqual(">")) {
                    return Token(Token::Type::OperatorArrow, "->");
                }

                return Token(Token::Type::OperatorMinus, "-");
            case '+':
                if (this->subtokenAdvanceIfEqual("+")) {
                    return Token(Token::Type::OperatorIncrement, "++");
                }

                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorAddAssign, "+=");
                }

                return Token(Token::Type::OperatorPlus, "+");
            case '%':
                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorModAssign, "%=");
                }

                return Token(Token::Type::OperatorMod, "%");
            case '/':
                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorDivideAssign, "/=");
                }

                return Token(Token::Type::OperatorDivide, "/");
            case '*':
                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorMultiplyAssign, "*=");
                }
            
                return Token(Token::Type::OperatorStar, "*");    
            case '=':
                if (this->subtokenAdvanceIfEqual("=")) {
                    if (this->subtokenAdvanceIfEqual("=")) {
                        return Token(Token::Type::OperatorDeepEqual, "===");
                    }

                    return Token(Token::Type::OperatorEqual, "==");
                }

                return Token(Token::Type::OperatorAssign, "=");
            case '<':
                if (this->subtokenAdvanceIfEqual("=")) {
                    if (this->subtokenAdvanceIfEqual(">")) {
                        return Token(Token::Type::OperatorCompare, "<=>");
                    }

                    return Token(Token::Type::OperatorLessOrEqual, "<=");
                }

                if (this->subtokenAdvanceIfEqual("<")) {
                    if (this->subtokenAdvanceIfEqual("=")) {
                        return Token(Token::Type::OperatorLeftShiftAssign, "<<=");
                    }

                    return Token(Token::Type::OperatorLeftShift, "<<");
                }

                return Token(Token::Type::OperatorLessThan, "<");
            case '!':
                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorNotEqual, "!=");
                }

                return Token(Token::Type::OperatorNot, "!");
            case '>':
                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorGreaterOrEqual, ">=");
                }

                if (this->subtokenAdvanceIfEqual(">")) {
                    if (this->subtokenAdvanceIfEqual("=")) {
                        return Token(Token::Type::OperatorRightShiftAssign, ">>=");
                    }

                    return Token(Token::Type::OperatorRightShift, ">>");
                }

                return Token(Token::Type::OperatorGreaterThan, ">");
            case '&':
                if (this->subtokenAdvanceIfEqual("&")) {
                    if (this->subtokenAdvanceIfEqual("=")) {
                        return Token(Token::Type::OperatorLogicalAndAssign, "&&=");
                    }

                    return Token(Token::Type::OperatorLogicalAnd, "&&");
                }

                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorBitwiseAndAssign, "&=");
                }

                return Token(Token::Type::OperatorAmpersand, "&");
            case '|':
                if (this->subtokenAdvanceIfEqual("|")) {
                    if (this->subtokenAdvanceIfEqual("=")) {
                        return Token(Token::Type::OperatorLogicalOrAssign, "||=");
                    }

                    return Token(Token::Type::OperatorLogicalOr, "||");
                }

                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorBitwiseOrAssign, "|=");
                }

                return Token(Token::Type::OperatorBinaryOr, "|");
            case '^':
                if (this->subtokenAdvanceIfEqual("^")) {
                    return Token(Token::Type::OperatorLogicalXor, "^^");
                }

                if (this->subtokenAdvanceIfEqual("=")) {
                    return Token(Token::Type::OperatorBitwiseXorAssign, "^=");
                }

                return Token(Token::Type::OperatorBinaryXor, "^");
            default:
                break;
        }

        return Token();
    }

    Token NewLexer::lexAnnotation() {
        Token::Type type = Token::Type::Undefined;

        assert(this->subtokenAdvance().str() == "@");

        Subtoken t = this->subtokenAdvance();

        if (_annotationMap.find(t.str()) != _annotationMap.end()) {
            type = _annotationMap[t.str()];
        }

        return Token(type, "@" + t.str());
    }
}
