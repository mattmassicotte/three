#pragma once

#include "../CodeGen/CSource.h"

#include <string>
#include <vector>

namespace Language {
    class DataType;
}

namespace Three {
    class Module;
}

namespace Language {
    class TypeReference {
    public:
        static TypeReference ref(Three::Module* module, const std::string& name, uint32_t indirection);

    public:
        TypeReference();
        TypeReference(DataType* referencedType, uint32_t indirection);

        std::string str() const;

        DataType* referencedType() const;
        void setReferencedType(DataType* type);
        std::string name() const;
        uint32_t indirectionDepth() const;
        void setIndirectionDepth(uint32_t depth);
        void incrementIndirectionDepth();
        bool isArray() const;
        std::vector<uint32_t> arrayDimensions() const;

        void codeGenCSourceFunctionType(CSource* source, const std::string& variableName) const;
        void codeGenCSource(CSource* source, const std::string& variableName) const;

    private:
        DataType* _type;
        uint32_t  _indirection;

        std::vector<uint32_t> _arrayDimensions;
    };
}
