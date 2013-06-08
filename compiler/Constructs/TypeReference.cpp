#include "TypeReference.h"
#include "Module.h"
#include "DataType.h"

#include <assert.h>

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

    void TypeReference::incrementIndirectionDepth() {
        _indirection++;
    }

    void TypeReference::codeGenCSource(CSource* source, const std::string& variableName) const {
        if (_type->flavor() != DataType::Flavor::Function) {
            source->print(_type->cSourceName());
            source->print(std::string(this->indirectionDepth(), '*'));

            if (variableName.size() > 0) {
                source->print(" ");
                source->print(variableName);
            }

            return;
        }

        // return (*varName)(param1, param2);
        _type->returnType().codeGenCSource(source, "");
        source->print(" (");
        source->print(std::string(this->indirectionDepth(), '*'));
        if (variableName.size() > 0) {
            source->print(variableName);
        }
        source->print(")(");

        // our environment pointer
        if (_type->flavor() == DataType::Flavor::Closure) {
            source->print("void*");
            if (_type->childCount() > 0) {
                source->print(", ");
            }
        }

        // parameters
        _type->eachParameterWithLast([=] (const TypeReference& param, bool last) {
            param.codeGenCSource(source, "");
            if (!last) {
                source->print(", ");
            }
        });

        source->print(")");
    }

}
