#pragma once

#include "QualifiedName.h"

#include <string>
#include <vector>
#include <functional>

namespace Three {
    class DataType {
    public:
        typedef enum {
            Undefined,
            Unknown,
            Void,
            Integer,
            Boolean,
            Natural,
            Float,
            Real,
            Character,
            Pointer,
            NullablePointer,
            Tuple,
            Array,
            Function,
            Closure,
            Structure,
            Enumeration,
            Union,
            Vararg,
            Generic,
            GenericArray,
            GenericPointer,
            GenericNullablePointer,
            CFunction,
            CStructure,
            CStructPrefixedStructure,
            CChar,
            CUnsignedChar,
            CShort,
            CUnsignedShort,
            CInt,
            CUnsignedInt,
            CLong,
            CUnsignedLong,
            CLongLong,
            CUnsignedLongLong,
            CUnspecifiedMacro
        } Kind;

        typedef enum {
            ASCII,
            UTF8,
            UTF16,
            UTF16BE,
            UTF16LE,
            UTF32,
            UTF32LE,
            UTF32BE
        } CharacterEncoding;

        typedef enum {
            None,
            Read,
            Write,
            ReadWrite
        } Access;

    public:
        static DataType wrapInType(Kind k, const DataType& subtype, const std::string& label = "");
        static DataType wrapInPointer(const DataType& type, const std::string& label = "");
        static DataType mutableVersion(const DataType& type);

    public:
        DataType(Kind k=Kind::Undefined);

    public:
        QualifiedName qualifiedName() const;
        std::string name() const;
        void setName(const QualifiedName& value);
        bool hasName() const;
        Kind kind() const;
        void setKind(Kind k);

        bool defined() const;
        bool isPointer() const;
        bool isCScalar() const;

        uint32_t widthSpecifier() const;
        void setWidthSpecifier(uint32_t width);
        uint32_t alignmentSpecifier() const;
        void setAlignmentSpecifier(uint32_t align);
        uint32_t vectorSizeSpecifier() const;
        void setVectorSizeSpecifier(uint32_t size);

        CharacterEncoding characterEncoding() const;
        void setCharacterEncoding(CharacterEncoding encoding);

        uint32_t arrayCount() const;
        void setArrayCount(uint32_t count);

        Access access() const;
        void setAccess(Access a);
        Access volatility() const;
        void setVolatility(Access v);
        bool potentiallyAliased() const;
        void setPotentiallyAliased(bool a);

        std::string label() const;
        void setLabel(const std::string& value);

        uint32_t subtypeCount() const;
        DataType subtypeAtIndex(uint32_t idx) const;
        void addSubtype(DataType t);
        void eachSubtypeWithLast(std::function<void (const DataType&, bool)> func) const;
        DataType subtypeWithLabel(const std::string& label) const;

        uint32_t parameterCount() const;
        DataType parameterAtIndex(uint32_t idx) const;
        void addParameter(DataType t);
        void eachParameterWithLast(std::function<void (const DataType&, bool)> func) const;
        DataType returnType() const;
        uint32_t returnCount() const;
        DataType returnAtIndex(uint32_t idx) const;
        void addReturn(DataType t);
        void eachReturnWithLast(std::function<void (const DataType&, bool)> func) const;

        bool isGeneric() const;
        uint32_t genericParameterCount() const;
        DataType genericParameterAtIndex(uint32_t idx) const;
        void addGenericParameter(DataType t);

    public:
        std::vector<DataType> subtypes;
        std::vector<DataType> parameters;

    private:
        Kind _kind;
        QualifiedName _name;
        std::string _label;

        uint32_t _widthSpecifier;
        uint32_t _alignmentSpecifier;
        uint32_t _vectorSizeSpecifier;

        CharacterEncoding _characterEncoding;
        uint32_t _arrayCount;

        Access _access;
        Access _volatility;
        bool _potentiallyAliased;

        // For functions/closures
        std::vector<DataType> _returns;

        std::vector<DataType> _genericParameters;
    };
}
