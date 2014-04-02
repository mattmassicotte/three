#pragma once

#include "compiler/constructs/NewDataType.h"

namespace Three {
    class CTypeCodeGenerator {
    public:
        static std::string codeGen(const NewDataType& type, const std::string& varName="");
        static std::string codeGenScalarType(const NewDataType& type);
        static std::string codeGenFunctionSignature(const NewDataType& type, uint32_t indirectionDepth);
    };
}
