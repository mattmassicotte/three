#pragma once

#include "../CodeGen/CSource.h"

#include <string>
#include <vector>

namespace Three {
    class DataType;
    class Module;
    class CSourceContext;
}

namespace Three {
    class TypeReference {
    public:
        static TypeReference ref(Three::Module* module, const std::string& name, uint32_t indirection);

    public:
        TypeReference(DataType* referencedType = nullptr, uint32_t indirection = 0, const std::vector<uint32_t>& dimensions = std::vector<uint32_t>());

        std::string str() const;

        DataType* referencedType() const;
        void setReferencedType(DataType* type);
        std::string name() const;
        uint32_t indirectionDepth() const;
        void setIndirectionDepth(uint32_t depth);
        void incrementIndirectionDepth();
        bool isArray() const;
        std::vector<uint32_t> arrayDimensions() const;
        void setCSourcePrependStructKeyword(bool prepend);
        bool cSourcePrependStructKeyword() const;

        std::string codeGen(const std::string& variableName = std::string()) const;
        std::string codeGenFunction(const std::string& variableName) const;

    private:
        DataType* _type;
        uint32_t  _indirection;
        bool _prependsStructKeyword;

        std::vector<uint32_t> _arrayDimensions;
    };
}
