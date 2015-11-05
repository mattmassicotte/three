#include "TypeParser.h"
#include "compiler/Constructs/QualifiedName.h"

namespace Three {
    TypeParser::TypeParser(ParseHelper* helper, ParseContext* context) : _helper(helper), _context(context) {
        _characterEncodingMap["ascii"]   = DataType::CharacterEncoding::ASCII;
        _characterEncodingMap["utf8"]    = DataType::CharacterEncoding::UTF8;
        _characterEncodingMap["utf16"]   = DataType::CharacterEncoding::UTF16;
        _characterEncodingMap["utf16le"] = DataType::CharacterEncoding::UTF16LE;
        _characterEncodingMap["utf16be"] = DataType::CharacterEncoding::UTF16BE;
        _characterEncodingMap["utf32"]   = DataType::CharacterEncoding::UTF32;
        _characterEncodingMap["utf32le"] = DataType::CharacterEncoding::UTF32LE;
        _characterEncodingMap["utf32be"] = DataType::CharacterEncoding::UTF32BE;
    }

    bool TypeParser::peekQualifiedName(uint32_t* peekDepth, QualifiedName& peekedName) {
        assert(peekDepth);

        for (;;) {
            // We have to lead with an identifier
            if (_helper->peek(*peekDepth).type() != Token::Type::Identifier) {
                return false;
            }

            peekedName.appendComponent(_helper->peek(*peekDepth).str());
            *peekDepth += 1;

            // check for "::"
            if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
                break;
            }

            if (_helper->peek(*peekDepth + 1).type() != Token::Type::PunctuationColon) {
                break;
            }

            // Check for the empty specifier case of "Int::4"
            if (_helper->peek(*peekDepth + 2).type() != Token::Type::Identifier) {
                break;
            }

            // move past the "::", and check for the next identifier part
            *peekDepth += 2;
        }

        // We've treated every identifier as a namespace, because its convenient. But,
        // the last component is really the identifier itself (ie the name).
        peekedName.shiftLastComponentToName();

        return true;
    }

    QualifiedName TypeParser::parseQualifiedName(bool adjustForIdentifier) {
        QualifiedName name;

        for (;;) {
            // have to start with an identifier
            if (_helper->peek().type() != Token::Type::Identifier) {
                assert(0 && "Message: Invalid multi-part identifier");
            }

            name.appendComponent(_helper->nextStr());

            // need two successive colons to continue
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                break;
            }

            if (_helper->peek(2).type() != Token::Type::PunctuationColon) {
                break;
            }

            // check for and handle the case of empty specifiers
            if (_helper->peek(3).type() != Token::Type::Identifier) {
                break;
            }

            _helper->next();
            _helper->next();
        }

        if (adjustForIdentifier) {
            // do an adjustment to seperate the name from the namspace parts
            name.shiftLastComponentToName();
        }

        return name;
    }

    bool TypeParser::peekTypePrefixes(uint32_t* peekDepth) {
        assert(peekDepth);

        uint32_t originalDepth = *peekDepth;

        // Pointers, arrays, annotations could be intermixed.
        for (;;) {
            switch (_helper->peek(*peekDepth).type()) {
                case Token::Type::OperatorStar:
                    *peekDepth += 1;
                    this->peekTypePostfixes(peekDepth);
                    continue;
                case Token::Type::PunctuationOpenBracket:
                    *peekDepth += 1;

                    if (_helper->peek(*peekDepth).type() != Token::Type::LiteralInteger) {
                        return false;
                    }
                    *peekDepth += 1;

                    if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationCloseBracket) {
                        return false;
                    }
                    *peekDepth += 1;

                    continue;
                case Token::Type::AnnotationAccess:
                case Token::Type::AnnotationVolatile:
                case Token::Type::AnnotationAlias:
                case Token::Type::AnnotationConst:
                case Token::Type::AnnotationRestrict:
                    this->peekTypeAnnotations(peekDepth);
                    continue;
                default:
                    return false;
            }

            break; // kill the loop
        }

        // return true if we've advanced at all
        return *peekDepth > originalDepth;
    }

    bool TypeParser::peekTypePostfixes(uint32_t* peekDepth) {
        assert(peekDepth);

        unsigned int originalDepth = *peekDepth;

        for (;;) {
            switch (_helper->peek(*peekDepth).type()) {
                case Token::Type::OperatorNot:
                case Token::Type::OperatorQuestionMark:
                    *peekDepth += 1;
                    continue;
                default:
                    break;
            }

            break; // kill the loop
        }

        // return true if we've advanced at all
        return *peekDepth > originalDepth;
    }

    bool TypeParser::peekTypeAnnotations(uint32_t* peekDepth) {
        unsigned int originalDepth = *peekDepth;

        for (;;) {
            switch (_helper->peek(*peekDepth).type()) {
                case Token::Type::AnnotationConst:
                case Token::Type::AnnotationRestrict:
                    // these two area easy, because they take no arguments
                    *peekDepth += 1;
                    continue;
                case Token::Type::AnnotationVolatile:
                    // volatile can appear with no argument
                    if (_helper->peek(*peekDepth+1).type() != Token::Type::PunctuationOpenParen) {
                        *peekDepth += 1;
                        continue;
                    }
                    // intentional fall-through
                case Token::Type::AnnotationAccess:
                case Token::Type::AnnotationAlias:
                    if (_helper->peek(*peekDepth+1).type() != Token::Type::PunctuationOpenParen) {
                        return false;
                    }

                    // TODO: this isn't particularly strict syntax checking

                    if (_helper->peek(*peekDepth+3).type() != Token::Type::PunctuationCloseParen) {
                        return false;
                    }

                    *peekDepth += 4;
                    continue;
                default:
                    return false;
            }

            break;
        }

        return *peekDepth > originalDepth;
    }
    
    bool TypeParser::isAtType() {
        uint32_t depth = 1;

        return this->peekDepthIfAtType(depth) > 0;
    }

    uint32_t TypeParser::peekDepthIfAtType(uint32_t peekDepth) {
        if (this->peekType(&peekDepth)) {
            return peekDepth;
        }

        return 0;
    }

    bool TypeParser::peekType(uint32_t* peekDepth) {
        this->peekTypePrefixes(peekDepth);

        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
                return this->peekFunctionOrTupleType(peekDepth);
            default:
                if (this->peekSimpleType(peekDepth)) {
                    this->peekTypePostfixes(peekDepth);
                    return true;
                }
                return false;
        }

        assert(0 && "Unhandled peekType case");
    }

    bool TypeParser::peekFunctionOrTupleType(uint32_t* peekDepth) {
        // could be a tuple
        // "() <identifier>"

        // "{} -> Type <identifier>"
        // "() -> Type <identifier>"

        // first, figure out if we have a closure or function, and what
        // the ending punctuation is
        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::PunctuationOpenParen:
            case Token::Type::PunctuationOpenBrace:
                break;
            default:
                return false;
        }

        const Token::Type closingPunctuation = _helper->peek(*peekDepth).closingCounterpart();

        *peekDepth += 1;

        // We need to advance past the parameters.
        const bool maybeATuple = closingPunctuation == Token::Type::PunctuationCloseParen;

        if (!this->peekTypeList(peekDepth, closingPunctuation, maybeATuple)) {
            return false;
        }

        // closing punctionation found
        *peekDepth += 1;

        // check for the arrow
        if (_helper->peek(*peekDepth).type() != Token::Type::OperatorArrow) {
            // this is a tuple
            return true;
        }

        *peekDepth += 1;

        return this->peekType(peekDepth);
    }

    bool TypeParser::peekTypeList(uint32_t* peekDepth, Token::Type closingToken, bool allowLabel) {
        bool mustPeekType = false;

        // Just for safety, add a sanity check to prevent loops.
        for (int i = 0;; ++i) {
            assert(i < 100 && "Trapped potential infinite loop checking for list of types");

            if (!mustPeekType && _helper->peek(*peekDepth).type() == closingToken) {
                return true;
            }

            // there must be a type here
            if (!this->peekType(peekDepth)) {
                return false;
            }

            mustPeekType = false;

            // here, there might be a label
            if (allowLabel && _helper->peek(*peekDepth).type() == Token::Type::Identifier) {
                *peekDepth += 1;
            }

            // advance past the comma, and continue
            if (_helper->peek(*peekDepth).type() == Token::Type::PunctuationComma) {
                *peekDepth += 1;

                // prevent exit if we encounter the closing token
                mustPeekType = true;
            }
        }

        return false;
    }

    bool TypeParser::peekSimpleType(uint32_t* peekDepth) {
        if (_helper->peek(*peekDepth).type() == Token::Type::KeywordVararg) {
            *peekDepth += 1;
            return true;
        }

        // We have to lead with an identifier. We also need to do a little
        // dance with the peekDepth, because we could be at an identifier
        // that isn't a type, and that will change peekDepth;
        QualifiedName name;
        unsigned int depth = *peekDepth;
        if (!this->peekQualifiedName(&depth, name)) {
            return false;
        }

        if (!_context->definesTypeWithName(name)) {
            return false;
        }

        // assign the depth value, now that we have a confirmed type
        *peekDepth = depth;

        if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
            return true;
        }

        *peekDepth += 1;

        // First specifier, could be an identifier, a number, or another colon
        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::Identifier:
            case Token::Type::LiteralInteger:
                *peekDepth += 1;
                break;
            case Token::Type::PunctuationColon:
                break;
            default:
                return false; // not a type
        }

        if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
            return true;
        }

        *peekDepth += 1;

        // Second specifier, could be a number or another colon
        switch (_helper->peek(*peekDepth).type()) {
            case Token::Type::LiteralInteger:
                *peekDepth += 1;
                break;
            case Token::Type::PunctuationColon:
                break;
            default:
                return false; // not a type
        }

        if (_helper->peek(*peekDepth).type() != Token::Type::PunctuationColon) {
            return true;
        }

        *peekDepth += 1;

        // Third specifier - must be a number
        if (_helper->peek(*peekDepth).type() != Token::Type::LiteralInteger) {
            return false;
        }

        *peekDepth += 1;

        return true;
    }

    DataType TypeParser::parseType(bool genericParam) {
        // Handle recursive types (pointer and array)
        switch (_helper->peek().type()) {
            case Token::Type::OperatorStar:
                return this->parsePointerType(genericParam);
            case Token::Type::PunctuationOpenBracket:
                return this->parseArrayType(genericParam);
            case Token::Type::PunctuationOpenParen:
                return this->parseFunctionOrTupleType();
            case Token::Type::PunctuationOpenBrace:
                return this->parseClosureType();
            case Token::Type::KeywordVararg:
                _helper->next();
                return DataType(DataType::Kind::Vararg);
            default:
                break;
        }

        QualifiedName name = this->parseQualifiedName();
        if (!name.valid()) {
            assert(0 && "Message: unable to parse qualified name as part of a type");
        }

        DataType type;

        if (genericParam) {
            type = DataType(DataType::Kind::Generic);
            type.setName(name.to_s());
        } else {
            type = _context->dataTypeForName(name.to_s());
        }

        type = this->parseTypeSpecifiers(type);

        return this->applyTypePostfixes(type, genericParam);
    }

    DataType TypeParser::parseTypeSpecifiers(const DataType& type) {
        if (type.kind() == DataType::Kind::Character) {
            return this->parseCharacterTypeSpecifiers(type);
        }

        DataType newType(type);

        uint32_t value = 0;

        // width specifier (or, colon)
        if (_helper->nextIf(Token::Type::PunctuationColon)) {
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                if (!this->parseIntegerSpecifierValue(&value)) {
                    return DataType();
                }

                newType.setWidthSpecifier(value);
            }
        }

        // alignment (or, colon)
        if (_helper->nextIf(Token::Type::PunctuationColon)) {
            if (_helper->peek().type() != Token::Type::PunctuationColon) {
                if (!this->parseIntegerSpecifierValue(&value)) {
                    return DataType();
                }

                newType.setAlignmentSpecifier(value);
            }
        }

        // vector
        if (_helper->nextIf(Token::Type::PunctuationColon)) {
            if (!this->parseIntegerSpecifierValue(&value)) {
                return DataType();
            }

            newType.setVectorSizeSpecifier(value);
        }

        return newType;
    }

    DataType TypeParser::parseCharacterTypeSpecifiers(const DataType& type) {
        if (!_helper->nextIf(Token::Type::PunctuationColon)) {
            return type;
        }

        assert(_helper->peek().type() == Token::Type::Identifier);

        auto it = _characterEncodingMap.find(_helper->next().str());

        if (it == _characterEncodingMap.cend()) {
            assert(0 && "Message about invalid character encoding");
            return DataType();
        }

        DataType newType(type);

        newType.setCharacterEncoding(it->second);

        return newType;
    }

    bool TypeParser::parseIntegerSpecifierValue(uint32_t* value) {
        if (_helper->peek().type() != Token::Type::LiteralInteger) {
            assert(0 && "Message: width specifier being invalid");
            return false;
        }

        *value = strtol(_helper->next().str().c_str(), NULL, 10);

        return true;
    }

    DataType TypeParser::applyTypePostfixes(const DataType& originalType, bool genericParam) {
        DataType type(originalType);

        if (_helper->nextIf(Token::Type::OperatorNot)) {
            type.setAccess(DataType::Access::ReadWrite);
        }

        if (_helper->nextIf(Token::Type::OperatorQuestionMark)) {
            DataType::Kind kind = genericParam ? DataType::Kind::GenericNullablePointer : DataType::Kind::NullablePointer;
            DataType pointerType(kind);

            pointerType.addSubtype(type);

            type = pointerType;
        }

        return type;
    }

    DataType TypeParser::parsePointerType(bool genericParam) {
        assert(_helper->next().type() == Token::Type::OperatorStar);

        DataType type = DataType(genericParam ? DataType::Kind::GenericPointer : DataType::Kind::Pointer);

        type = this->parseTypePostfixes(type);

        type.addSubtype(this->parseType(genericParam));

        return this->applyTypePostfixes(type, genericParam);
    }

    DataType TypeParser::parseArrayType(bool genericParam) {
        assert(_helper->next().type() == Token::Type::PunctuationOpenBracket);

        DataType type = DataType(genericParam ? DataType::Kind::GenericArray : DataType::Kind::Array);

        if (_helper->peek().type() != Token::Type::PunctuationCloseBracket) {
            uint32_t value = 0;

            if (!this->parseIntegerSpecifierValue(&value)) {
                assert(0 && "Message: unable to parse array size");
                return DataType();
            }

            type.setArrayCount(value);
        }

        if (!_helper->nextIf(Token::Type::PunctuationCloseBracket)) {
            assert(0 && "Message: array type element count should be followed by a close bracket");
            return DataType();
        }

        type.addSubtype(this->parseType(genericParam));

        return this->applyTypePostfixes(type, genericParam);
    }

    DataType TypeParser::parseTypePostfixes(const DataType& type) {
        DataType newType(type);

        if (_helper->nextIf(Token::Type::OperatorNot)) {
            newType.setAccess(DataType::Access::ReadWrite);
        }

        if (_helper->nextIf(Token::Type::OperatorQuestionMark)) {
            switch (type.kind()) {
                case DataType::Kind::Pointer:
                    newType.setKind(DataType::Kind::NullablePointer);
                    break;
                case DataType::Kind::GenericPointer:
                    newType.setKind(DataType::Kind::GenericNullablePointer);
                    break;
                case DataType::Kind::Generic:
                    newType = DataType::wrapInType(DataType::Kind::GenericNullablePointer, newType);
                    break;
                default:
                    newType = DataType::wrapInType(DataType::Kind::NullablePointer, newType);
                    break;
            }
        }

        return newType;
    }

    DataType TypeParser::parseFunctionOrTupleType() {
        assert(_helper->nextIf(Token::Type::PunctuationOpenParen));

        DataType type;

        this->parseTypeListWithLabels(Token::Type::PunctuationCloseParen, [&] (const DataType& subtype) {
            type.addSubtype(subtype);
        });

        if (!_helper->nextIf(Token::Type::PunctuationCloseParen)) {
            assert(0 && "Message: failed to parse a tuple/function type");

            return DataType();
        }

        if (!_helper->nextIf(Token::Type::OperatorArrow)) {
            type.setKind(DataType::Kind::Tuple);

            return type;
        }

        type.setKind(DataType::Kind::Function);

        // TODO: clean this up - move the subtypes to parameters
        type.parameters = type.subtypes;
        type.subtypes.clear();

        type.setReturnType(this->parseType());

        return type;
    }

    DataType TypeParser::parseClosureType() {
        assert(_helper->nextIf(Token::Type::PunctuationOpenBrace));

        DataType type(DataType::Kind::Closure);

        // make sure to define the environment pointer
        type.addParameter(DataType::wrapInPointer(DataType::Kind::Void));

        this->parseTypeList(Token::Type::PunctuationCloseBrace, true, [&] (const DataType& subtype) {
            type.addParameter(subtype);
        });

        if (!_helper->nextIf(Token::Type::PunctuationCloseBrace)) {
            assert(0 && "Message: failed to parse a closure type, missing close brace");

            return DataType();
        }

        if (!_helper->nextIf(Token::Type::OperatorArrow)) {
            type.setReturnType(DataType(DataType::Kind::Void));

            return type;
        }

        type.setReturnType(this->parseType());

        return type;
    }

    bool TypeParser::parseTypeListWithLabels(Token::Type closingToken, std::function<void (const DataType& type)> func) {
        return this->parseTypeList(closingToken, true, func);
    }

    bool TypeParser::parseTypeList(Token::Type closingToken, bool allowIdentifiers, std::function<void (const DataType& type)> func) {
        // the type list could be empty
        if (_helper->peek().type() == closingToken) {
            return true;
        }

        for (int i = 0;; ++i) {
            assert(i < 100 && "Trapped potential infinite loop parsing list of types");

            if (!this->isAtType()) {
                break;
            }

            DataType type = this->parseType();

            if (allowIdentifiers && _helper->peek().type() == Token::Type::Identifier) {
                type.setLabel(_helper->nextStr());
            }

            func(type);

            if (_helper->nextIf(Token::Type::PunctuationComma)) {
                continue;
            }

            if (_helper->peek().type() == closingToken) {
                return true;
            }
        }

        // failed to parse the list as this point

        return false;
    }
}
