#pragma once

#include "compiler/constructs/DataType.h"

namespace Three {
    class CTypeCodeGenerator {
    public:
        static std::string codeGen(const DataType& type, const std::string& varName="");
        static std::string innerStringForType(const DataType& type);
        static std::string codeGenType(const DataType& type);
        static std::string codeGenScalarType(const DataType& type);
        static std::string codeGenCScalarType(const DataType& type);
        static std::string codeGenFunction(const DataType& type, const std::string& leadingString);
        static std::string codeGenFunctionType(const DataType& type, const std::string& innerString);
        static std::string codeGenTupleStructName(const DataType& type);

        static bool typeCanBeConst(const DataType& type);
    };
}
