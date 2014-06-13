#pragma once

#include <string>
#include <vector>
#include <functional>

namespace Three {
    class NewDataType {
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
            Array,
            Function,
            Closure,
            Structure,
            Enumeration,
            Union,
            Vararg,
            CFunction,
            CStructPrefixedStructure,
            CInt
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
        static NewDataType wrapInPointer(const NewDataType& type, const std::string& label = "");

    public:
        NewDataType(Kind k=Kind::Undefined);

    public:
        std::string name() const;
        void setName(const std::string& value);
        Kind kind() const;
        void setKind(Kind k);

        bool defined() const;
        bool isPointer() const;

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
        NewDataType subtypeAtIndex(uint32_t idx) const;
        void addSubtype(NewDataType t);

        uint32_t parameterCount() const;
        NewDataType parameterAtIndex(uint32_t idx) const;
        void addParameter(NewDataType t);
        void eachParameterWithLast(std::function<void (const NewDataType&, bool)> func) const;
        uint32_t returnCount() const;
        NewDataType returnAtIndex(uint32_t idx) const;
        void addReturn(NewDataType t);
        void eachReturnWithLast(std::function<void (const NewDataType&, bool)> func) const;

    public:
        std::vector<NewDataType> subtypes;
        std::vector<NewDataType> parameters;

    private:
        Kind _kind;
        std::string _name;
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
        std::vector<NewDataType> _returns;
    };
}
