#include "Token.h"

namespace Three {
    Token::Token() : _type(Type::Undefined) {
    }

    Token::Token(Token::Type type) : _type(type) {
    }

    Token::Token(Token::Type type, const std::string& value) : _type(type), _value(value) {
    }

    Token::Type Token::type() const {
        return _type;
    }

    std::string Token::str() const {
        return _value;
    }

    uint32_t Token::precedence() const {
        switch (_type) {
            case PunctuationOpenBracket: return 16;
            case OperatorIncrement:      return 16; // postfix only
            case OperatorDecrement:      return 16; // postfix only
            case OperatorDot:            return 16;
            case OperatorArrow:          return 16;

            case OperatorStar:           return 15; // infix
            case OperatorMod:            return 15; // infix
            case OperatorDivide:         return 15; // infix

            case OperatorPlus:           return 14;
            case OperatorMinus:          return 14;

            case OperatorLeftShift:      return 13;
            case OperatorRightShift:     return 13;

            case OperatorLessThan:       return 12;
            case OperatorLessOrEqual:    return 12;
            case OperatorGreaterThan:    return 12;
            case OperatorGreaterOrEqual: return 12;

            case OperatorEqual:          return 11;
            case OperatorNotEqual:       return 11;

            case OperatorAmpersand:      return 10; // binary and

            case OperatorBinaryOr:       return 9;

            case OperatorLogicalAnd:     return 8;

            case OperatorLogicalOr:      return 7;

            case OperatorQuestionMark:   return 6;
            case OperatorCAS:            return 6;

            case OperatorAssign:         return 5;
            case OperatorAddAssign:      return 5;
            case OperatorSubtractAssign: return 5;
            case OperatorMultiplyAssign: return 5;
            case OperatorDivideAssign:   return 5;
            case OperatorLogicalOrAssign: return 5;
            case OperatorLogicalAndAssign: return 5;

            default: break;
        }

        return Token::NonPrecedence;
    }

    bool Token::isEnd() {
        return _type == Token::Type::Undefined || _type == Token::Type::EndOfInput;
    }

    bool Token::isStatementEnding() {
        return this->isEnd() || _type == Token::Type::Newline;
    }

    bool Token::isAnnotation() const {
        switch (_type) {
            case AnnotationRead:
            case AnnotationWrite:
            case AnnotationAssert:
            case AnnotationSentinel:
            case AnnotationChecked:
            case AnnotationNull:
            case AnnotationSuccess:
            case AnnotationGlobal:
            case AnnotationThread:
            case AnnotationIO:
            case AnnotationFlow:
            case AnnotationAvailable:
            case AnnotationNoreturn:
            case AnnotationCommon:
            case AnnotationUncommon:
            case AnnotationOptimize:
            case AnnotationInline:
            case AnnotationPrefetch:
            case AnnotationMemory:
            case AnnotationPure:
            case AnnotationThrows:
            case AnnotationReleased:
                return true;
            default:
                break;
        }

        return false;
    }

    bool Token::isOperator() const {
        switch (_type) {
            case OperatorScope:
            case OperatorStar:
            case OperatorAmpersand:
            case OperatorMinus:
            case OperatorIncrement:
            case OperatorDecrement:
            case OperatorPlus:
            case OperatorDivide:
            case OperatorMod:
            case OperatorDot:
            case OperatorArrow:
            case OperatorEqual:
            case OperatorDeepEqual:
            case OperatorCompare:
            case OperatorNotEqual:
            case OperatorGreaterThan:
            case OperatorLessThan:
            case OperatorGreaterOrEqual:
            case OperatorLessOrEqual:
            case OperatorLogicalAnd:
            case OperatorLogicalOr:
            case OperatorLogicalXor:
            case OperatorNot:
            case OperatorBinaryNot:
            case OperatorBinaryOr:
            case OperatorBinaryXor:
            case OperatorLeftShift:
            case OperatorRightShift:
            case OperatorAssign:
            case OperatorAddAssign:
            case OperatorSubtractAssign:
            case OperatorMultiplyAssign:
            case OperatorDivideAssign:
            case OperatorModAssign:
            case OperatorBitwiseAndAssign:
            case OperatorBitwiseOrAssign:
            case OperatorBitwiseXorAssign:
            case OperatorLeftShiftAssign:
            case OperatorRightShiftAssign:
            case OperatorLogicalAndAssign:
            case OperatorLogicalOrAssign:
            case OperatorQuestionMark:
            case OperatorCAS:
                return true;
            default:
                break;
        }

        return false;
    }

    bool Token::isUnaryOperator() const {
        switch (_type) {
            case OperatorAmpersand:
            case OperatorStar:
            case OperatorMinus:
            case OperatorNot:
            case OperatorBinaryNot:
            case OperatorIncrement:
            case OperatorDecrement:
                return true;
            default:
                break;
        }

        return false;
    }

    bool Token::isTernaryOperator() const {
        switch (_type) {
            case OperatorQuestionMark:
            case OperatorCAS:
                return true;
            default:
                break;
        }

        return false;
    }

    bool Token::isLeftAssociative() const {
        // easier to just check for right-associativity
        switch (_type) {
            case OperatorIncrement:
            case OperatorDecrement:
            case OperatorMinus:
            case OperatorNot:
            case OperatorBinaryNot:
            case OperatorStar:
            case OperatorAmpersand:
            case OperatorAssign:
            case OperatorAddAssign:
            case OperatorSubtractAssign:
            case OperatorMultiplyAssign:
            case OperatorDivideAssign:
            case OperatorModAssign:
            case OperatorBitwiseAndAssign:
            case OperatorBitwiseOrAssign:
            case OperatorBitwiseXorAssign:
            case OperatorLeftShiftAssign:
            case OperatorRightShiftAssign:
            case OperatorLogicalAndAssign:
            case OperatorLogicalOrAssign:
                return false;
            default:
                break;
        }

        return true;
    }
}
    