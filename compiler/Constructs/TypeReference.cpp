#include "TypeReference.h"
#include "Module.h"

namespace Language {
    TypeReference TypeReference::ref(Module* module, const std::string& name, uint32_t indirection) {
        return TypeReference(module->dataTypeForName(name), indirection);
    }

    TypeReference::TypeReference() : _type(NULL), _indirection(0) {
    }

    TypeReference::TypeReference(DataType* referencedType, uint32_t indirection) : _type(referencedType), _indirection(indirection) {
    }

    DataType* TypeReference::referencedType() const {
        return _type;
    }

    void TypeReference::setReferencedType(DataType* type) {
        _type = type;
    }

    uint32_t TypeReference::indirectionDepth() const {
        return _indirection;
    }

    void TypeReference::setIndirectionDepth(uint32_t depth) {
        _indirection = depth;
    }
}
