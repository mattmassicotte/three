#include "NewDataType.h"

namespace Three {
    NewDataType NewDataType::wrapInType(Kind k, const NewDataType& subtype, const std::string& label) {
        NewDataType type(k);

        type.addSubtype(subtype);
        type.setLabel(label);

        return type;
    }

    NewDataType NewDataType::wrapInPointer(const NewDataType& type, const std::string& label) {
        return wrapInType(NewDataType::Kind::Pointer, type, label);
    }

    NewDataType NewDataType::mutableVersion(const NewDataType& type) {
        NewDataType newType(type);

        newType.setAccess(NewDataType::Access::ReadWrite);

        return newType;
    }

    NewDataType::NewDataType(Kind k) :
        _kind(k),
        _widthSpecifier(0),
        _alignmentSpecifier(0),
        _vectorSizeSpecifier(0),
        _arrayCount(0),
        _characterEncoding(NewDataType::CharacterEncoding::UTF8),
        _access(NewDataType::Access::Read),
        _volatility(NewDataType::Access::None),
        _potentiallyAliased(true) {
    }

    std::string NewDataType::name() const {
        if (_name.size() > 0) {
            return _name;
        }

        switch (_kind) {
            case Undefined: return "Undefined";
            case Integer:   return "Int";
            case Boolean:   return "Bool";
            default:        return "Unspecified";
        }
    }

    void NewDataType::setName(const std::string& value) {
        _name = value;
    }

    NewDataType::Kind NewDataType::kind() const {
        return _kind;
    }

    void NewDataType::setKind(Kind k) {
        _kind = k;
    }

    bool NewDataType::defined() const {
        return _kind != Kind::Undefined && _kind != Kind::Unknown;
    }

    bool NewDataType::isPointer() const {
        return _kind == Kind::Pointer || _kind == Kind::Function;
    }

    uint32_t NewDataType::widthSpecifier() const {
        return _widthSpecifier;
    }

    void NewDataType::setWidthSpecifier(uint32_t width) {
        _widthSpecifier = width;
    }

    uint32_t NewDataType::alignmentSpecifier() const {
        return _alignmentSpecifier;
    }

    void NewDataType::setAlignmentSpecifier(uint32_t align) {
        _alignmentSpecifier = align;
    }

    uint32_t NewDataType::vectorSizeSpecifier() const {
        return _vectorSizeSpecifier;
    }

    void NewDataType::setVectorSizeSpecifier(uint32_t size) {
        _vectorSizeSpecifier = size;
    }

    NewDataType::CharacterEncoding NewDataType::characterEncoding() const {
        return _characterEncoding;
    }

    void NewDataType::setCharacterEncoding(NewDataType::CharacterEncoding encoding) {
        _characterEncoding = encoding;
    }

    uint32_t NewDataType::arrayCount() const {
        return _arrayCount;
    }

    void NewDataType::setArrayCount(uint32_t count) {
        _arrayCount = count;
    }

    NewDataType::Access NewDataType::access() const {
        return _access;
    }

    void NewDataType::setAccess(NewDataType::Access a) {
        _access = a;
    }

    NewDataType::Access NewDataType::volatility() const {
        return _volatility;
    }

    void NewDataType::setVolatility(NewDataType::Access v) {
        _volatility = v;
    }

    bool NewDataType::potentiallyAliased() const {
        return _potentiallyAliased;
    }

    void NewDataType::setPotentiallyAliased(bool a) {
        _potentiallyAliased = a;
    }

    std::string NewDataType::label() const {
        return _label;
    }
    void NewDataType::setLabel(const std::string& value) {
        _label = value;
    }

    uint32_t NewDataType::subtypeCount() const {
        return subtypes.size();
    }

    NewDataType NewDataType::subtypeAtIndex(uint32_t idx) const {
        return subtypes.at(idx);
    }

    void NewDataType::addSubtype(NewDataType t) {
        subtypes.push_back(t);
    }

    void NewDataType::eachSubtypeWithLast(std::function<void (const NewDataType&, bool)> func) const {
        uint32_t lastIndex = this->subtypeCount() - 1;

        for (uint32_t i = 0; i < this->subtypeCount(); ++i) {
            func(this->subtypeAtIndex(i), lastIndex == i);
        }
    }

    uint32_t NewDataType::parameterCount() const {
        return parameters.size();
    }

    NewDataType NewDataType::parameterAtIndex(uint32_t idx) const {
        return parameters.at(idx);
    }

    void NewDataType::addParameter(NewDataType t) {
        parameters.push_back(t);
    }

    void NewDataType::eachParameterWithLast(std::function<void (const NewDataType&, bool)> func) const {
        uint32_t lastIndex = this->parameterCount() - 1;

        for (uint32_t i = 0; i < this->parameterCount(); ++i) {
            func(this->parameterAtIndex(i), lastIndex == i);
        }
    }

    NewDataType NewDataType::returnType() const {
        return _returns.at(0);
    }

    uint32_t NewDataType::returnCount() const {
        return _returns.size();
    }

    NewDataType NewDataType::returnAtIndex(uint32_t idx) const {
        return _returns.at(idx);
    }

    void NewDataType::addReturn(NewDataType t) {
        _returns.push_back(t);
    }

    void NewDataType::eachReturnWithLast(std::function<void (const NewDataType&, bool)> func) const {
        uint32_t lastIndex = this->returnCount() - 1;

        for (uint32_t i = 0; i < this->returnCount(); ++i) {
            func(this->returnAtIndex(i), lastIndex == i);
        }
    }

    bool NewDataType::isGeneric() const {
        return this->genericParameterCount() > 0;
    }

    uint32_t NewDataType::genericParameterCount() const {
        return _genericParameters.size();
    }

    NewDataType NewDataType::genericParameterAtIndex(uint32_t idx) const {
        return _genericParameters.at(idx);
    }

    void NewDataType::addGenericParameter(NewDataType t) {
        _genericParameters.push_back(t);
    }
}
