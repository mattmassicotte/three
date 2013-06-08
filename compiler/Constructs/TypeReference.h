#pragma once

#include "../CodeGen/CSource.h"

#include <string>

namespace Language {
    class DataType;
    class Module;
}

namespace Language {
    class TypeReference {
    public:
        static TypeReference ref(Module* module, const std::string& name, uint32_t indirection);

    public:
        TypeReference();
        TypeReference(DataType* referencedType, uint32_t indirection);

        DataType* referencedType() const;
        void setReferencedType(DataType* type);
        uint32_t indirectionDepth() const;
        void setIndirectionDepth(uint32_t depth);
        void incrementIndirectionDepth();

        void codeGenCSource(CSource* source, const std::string& variableName) const;

    private:
        DataType* _type;
        uint32_t  _indirection;
    };
}
