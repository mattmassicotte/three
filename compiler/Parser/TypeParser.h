#pragma once

#include "ParseHelper.h"
#include "ParseContext.h"

#include <map>

namespace Three {
    class Parser;
    class DataType;
    class QualifiedName;
}

namespace Three {
    class TypeParser {
    public:
        TypeParser(ParseHelper* helper, ParseContext* context);

        // TODO - this should be moved into their own parser class
        bool peekQualifiedName(uint32_t* peekDepth, QualifiedName& peekedName);
        // TODO - should not use a boolean value here
        QualifiedName parseQualifiedName(bool adjustForIdentifier = true);

        bool peekTypePrefixes(uint32_t* peekDepth);
        bool peekTypePostfixes(uint32_t* peekDepth);
        bool peekTypeAnnotations(uint32_t* peekDepth);

        uint32_t peekDepthIfAtType(uint32_t peekDepth = 1);
        bool peekType(uint32_t* peekDepth);
        bool peekFunctionOrTupleType(uint32_t* peekDepth);
        bool peekTypeList(uint32_t* peekDepth, Token::Type closingToken, bool allowLabels);
        bool peekSimpleType(uint32_t* peekDepth);

        bool isAtType();
        DataType parseType(bool genericParam = false);
        DataType parseTypeSpecifiers(const DataType& type);
        DataType parseCharacterTypeSpecifiers(const DataType& type);
        bool parseIntegerSpecifierValue(uint32_t* value);
        DataType applyTypePostfixes(const DataType& originalType, bool genericParam);
        DataType parsePointerType(bool genericParam);
        DataType parseArrayType(bool genericParam);
        DataType parseTypePostfixes(const DataType& type);
        DataType parseFunctionOrTupleType();
        DataType parseClosureType();
        bool parseTypeListWithLabels(Token::Type closingToken, std::function<void (const DataType& type)> func);
        bool parseTypeList(Token::Type closingToken, bool allowIdentifiers, std::function<void (const DataType& type)> func);

    private:
        ParseHelper* _helper;
        ParseContext* _context;

        std::map<std::string, DataType::CharacterEncoding> _characterEncodingMap;
    };
}
