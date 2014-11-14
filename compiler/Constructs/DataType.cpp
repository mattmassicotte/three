#include "DataType.h"

namespace Three {
    DataType DataType::wrapInType(Kind k, const DataType& subtype, const std::string& label) {
        DataType type(k);

        type.addSubtype(subtype);
        type.setLabel(label);

        return type;
    }

    DataType DataType::wrapInPointer(const DataType& type, const std::string& label) {
        return wrapInType(DataType::Kind::Pointer, type, label);
    }

    DataType DataType::mutableVersion(const DataType& type) {
        DataType newType(type);

        newType.setAccess(DataType::Access::ReadWrite);

        return newType;
    }

    DataType::DataType(Kind k) :
        _kind(k),
        _widthSpecifier(0),
        _alignmentSpecifier(0),
        _vectorSizeSpecifier(0),
        _arrayCount(0),
        _characterEncoding(DataType::CharacterEncoding::UTF8),
        _access(DataType::Access::Read),
        _volatility(DataType::Access::None),
        _potentiallyAliased(true) {
    }

    std::string DataType::name() const {
        if (this->hasName()) {
            return _name;
        }

        switch (_kind) {
            case Kind::Integer: return "Int";
            case Kind::Boolean: return "Bool";
            case Kind::Natural: return "Natural";
            case Kind::Float:   return "Float";
            case Kind::Real:    return "Real";
            default: break;
        }

        return "";
    }

    void DataType::setName(const std::string& value) {
        _name = value;
    }

    bool DataType::hasName() const {
        return _name.size() > 0;
    }

    DataType::Kind DataType::kind() const {
        return _kind;
    }

    void DataType::setKind(Kind k) {
        _kind = k;
    }

    bool DataType::defined() const {
        return _kind != Kind::Undefined && _kind != Kind::Unknown;
    }

    bool DataType::isPointer() const {
        switch (this->kind()) {
            case Kind::Pointer:
            case Kind::NullablePointer:
            case Kind::GenericPointer:
            case Kind::GenericNullablePointer:
                return true;
            default:
                break;
        }

        return false;
    }

    bool DataType::isCScalar() const {
        switch (this->kind()) {
            case Kind::CChar:
            case Kind::CUnsignedChar:
            case Kind::CShort:
            case Kind::CUnsignedShort:
            case Kind::CInt:
            case Kind::CUnsignedInt:
            case Kind::CLong:
            case Kind::CUnsignedLong:
            case Kind::CLongLong:
            case Kind::CUnsignedLongLong:
                return true;
            default:
                return false;
        }
    }

    uint32_t DataType::widthSpecifier() const {
        return _widthSpecifier;
    }

    void DataType::setWidthSpecifier(uint32_t width) {
        _widthSpecifier = width;
    }

    uint32_t DataType::alignmentSpecifier() const {
        return _alignmentSpecifier;
    }

    void DataType::setAlignmentSpecifier(uint32_t align) {
        _alignmentSpecifier = align;
    }

    uint32_t DataType::vectorSizeSpecifier() const {
        return _vectorSizeSpecifier;
    }

    void DataType::setVectorSizeSpecifier(uint32_t size) {
        _vectorSizeSpecifier = size;
    }

    DataType::CharacterEncoding DataType::characterEncoding() const {
        return _characterEncoding;
    }

    void DataType::setCharacterEncoding(DataType::CharacterEncoding encoding) {
        _characterEncoding = encoding;
    }

    uint32_t DataType::arrayCount() const {
        return _arrayCount;
    }

    void DataType::setArrayCount(uint32_t count) {
        _arrayCount = count;
    }

    DataType::Access DataType::access() const {
        return _access;
    }

    void DataType::setAccess(DataType::Access a) {
        _access = a;
    }

    DataType::Access DataType::volatility() const {
        return _volatility;
    }

    void DataType::setVolatility(DataType::Access v) {
        _volatility = v;
    }

    bool DataType::potentiallyAliased() const {
        return _potentiallyAliased;
    }

    void DataType::setPotentiallyAliased(bool a) {
        _potentiallyAliased = a;
    }

    std::string DataType::label() const {
        return _label;
    }
    void DataType::setLabel(const std::string& value) {
        _label = value;
    }

    uint32_t DataType::subtypeCount() const {
        return subtypes.size();
    }

    DataType DataType::subtypeAtIndex(uint32_t idx) const {
        return subtypes.at(idx);
    }

    void DataType::addSubtype(DataType t) {
        subtypes.push_back(t);
    }

    void DataType::eachSubtypeWithLast(std::function<void (const DataType&, bool)> func) const {
        uint32_t lastIndex = this->subtypeCount() - 1;

        for (uint32_t i = 0; i < this->subtypeCount(); ++i) {
            func(this->subtypeAtIndex(i), lastIndex == i);
        }
    }

    uint32_t DataType::parameterCount() const {
        return parameters.size();
    }

    DataType DataType::parameterAtIndex(uint32_t idx) const {
        return parameters.at(idx);
    }

    void DataType::addParameter(DataType t) {
        parameters.push_back(t);
    }

    void DataType::eachParameterWithLast(std::function<void (const DataType&, bool)> func) const {
        uint32_t lastIndex = this->parameterCount() - 1;

        for (uint32_t i = 0; i < this->parameterCount(); ++i) {
            func(this->parameterAtIndex(i), lastIndex == i);
        }
    }

    DataType DataType::returnType() const {
        return _returns.at(0);
    }

    uint32_t DataType::returnCount() const {
        return _returns.size();
    }

    DataType DataType::returnAtIndex(uint32_t idx) const {
        return _returns.at(idx);
    }

    void DataType::addReturn(DataType t) {
        _returns.push_back(t);
    }

    void DataType::eachReturnWithLast(std::function<void (const DataType&, bool)> func) const {
        uint32_t lastIndex = this->returnCount() - 1;

        for (uint32_t i = 0; i < this->returnCount(); ++i) {
            func(this->returnAtIndex(i), lastIndex == i);
        }
    }

    bool DataType::isGeneric() const {
        return this->genericParameterCount() > 0;
    }

    uint32_t DataType::genericParameterCount() const {
        return _genericParameters.size();
    }

    DataType DataType::genericParameterAtIndex(uint32_t idx) const {
        return _genericParameters.at(idx);
    }

    void DataType::addGenericParameter(DataType t) {
        _genericParameters.push_back(t);
    }
}
