#pragma once

#include "compiler/constructs/NewDataType.h"

namespace Three {
    class CTypeCodeGenerator {
    public:
        static std::string codeGen(const NewDataType& type, const std::string& varName="");
        static std::string innerStringForType(const NewDataType& type);
        static std::string codeGenType(const NewDataType& type);
        static std::string codeGenScalarType(const NewDataType& type);
        static std::string codeGenCScalarType(const NewDataType& type);
        static std::string codeGenFunction(const NewDataType& type, const std::string& leadingString);
        static std::string codeGenFunctionType(const NewDataType& type, const std::string& innerString);
        static std::string codeGenTupleStructName(const NewDataType& type);

        static bool typeCanBeConst(const NewDataType& type);
    };
}
