#include "CTypeCodeGenerator.h"

#include <sstream>

namespace Three {
    std::string CTypeCodeGenerator::codeGen(const NewDataType& type, const std::string& varName) {
        uint32_t indirectionDepth = 0;

        NewDataType usedType(type);

        // "dereference" down to a non-pointer type, which makes it easier
        // do to codegen
        while (usedType.kind() == NewDataType::Kind::Pointer) {
            assert(usedType.subtypeCount() == 1 && "Bug: Pointer types must have exactly one subtype");
            usedType = usedType.subtypeAtIndex(0);
            indirectionDepth += 1;
        }

        if (varName.size() > 0) {
            // In this case, we can use the variable name as the function's label
            usedType.setLabel(varName);
        } else {
            // otherwise, inherit the label
            usedType.setLabel(type.label());
        }

        std::stringstream s;

        if (usedType.access() == NewDataType::Access::Read) {
            s << "const ";
        }

        switch (usedType.kind()) {
            case NewDataType::Kind::Undefined: assert(0 && "Bug: cannot codegen an undefined type");
            case NewDataType::Kind::Unknown:   assert(0 && "Bug: cannot codegen an unknown type");
            case NewDataType::Kind::Void:
            case NewDataType::Kind::Integer:
            case NewDataType::Kind::Boolean:
            case NewDataType::Kind::Natural:
            case NewDataType::Kind::Float:
            case NewDataType::Kind::Real:
            case NewDataType::Kind::Character:
            case NewDataType::Kind::Vararg:
            case NewDataType::Kind::Tuple:
                s << CTypeCodeGenerator::codeGenScalarType(usedType);
                break;
            case NewDataType::Kind::Closure:
                // closures are a little special, because their type
                // is different when a signature and when a variable
                if (varName.size() > 0) {
                    s << CTypeCodeGenerator::codeGenScalarType(usedType);
                    break;
                }

                return CTypeCodeGenerator::codeGenFunctionSignature(usedType, indirectionDepth);
            case NewDataType::Kind::Pointer:
                assert(0 && "Bug: type should never be a pointer at this stage of codegen");
                break;
            case NewDataType::Kind::Array:
                break;
            case NewDataType::Kind::Function:
                return CTypeCodeGenerator::codeGenFunctionSignature(usedType, indirectionDepth);
            case NewDataType::Kind::Structure:
            case NewDataType::Kind::Enumeration:
            case NewDataType::Kind::Union:
                s << usedType.name();
                break;
            case NewDataType::Kind::CStructPrefixedStructure:
                s << "struct " << usedType.name();
                break;
        }

        s << std::string(indirectionDepth, '*');

        if (varName.size() > 0) {
            s << " " << varName;
        }

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenScalarType(const NewDataType& type) {
        std::stringstream s;

        switch (type.kind()) {
            case NewDataType::Kind::Void:
                return "void";
            case NewDataType::Kind::Integer:
                return "int";
            case NewDataType::Kind::Boolean:
                return "bool";
            case NewDataType::Kind::Natural:
                if (type.widthSpecifier() > 16 && type.widthSpecifier() <= 32) {
                    return "uint32_t";
                }

                return "unsigned int";
            case NewDataType::Kind::Float:
            case NewDataType::Kind::Real:
                if (type.widthSpecifier() > 32) {
                    return "double";
                } else {
                    return "float";
                }
            case NewDataType::Kind::Character:
                return "char";
            case NewDataType::Kind::Closure:
                return "three_closure_t";
            case NewDataType::Kind::Vararg:
                return "va_list";
            case NewDataType::Kind::Tuple:
                return CTypeCodeGenerator::codeGenTupleStructName(type);
            default:
                assert(0 && "Bug: unhandled scalar C type");
                break;
        }

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenFunctionSignature(const NewDataType& type, uint32_t indirectionDepth) {
        std::stringstream s;

        //assert(indirectionDepth == 0 && "Unhandled: indirectionDepth > 0");

        // returnType function(param1, param2)
        // returnType (*varName)(param1, param2)

        if (type.returnCount() == 0) {
            s << "void";
        } else {
            s << CTypeCodeGenerator::codeGen(type.returnAtIndex(0));
        }

        s << " ";

        assert(type.label().size() > 0);
        if (indirectionDepth > 0) {
            s << "(" << std::string(indirectionDepth, '*');
        }

        s << type.label();

        if (indirectionDepth > 0) {
            s << ")";
        }

        s << "(";

        if (type.parameterCount() == 0) {
            s << "void";
        } else {
            type.eachParameterWithLast([&s] (const NewDataType& paramType, bool last) {
                s << CTypeCodeGenerator::codeGen(paramType, paramType.label());
                if (!last) {
                    s << ", ";
                }
            });
            // TODO: parameters
        }

        s << ")";

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenTupleStructName(const NewDataType& type) {
        std::stringstream s;

        assert(type.subtypeCount() > 0);

        type.eachSubtypeWithLast([&] (const NewDataType& subtype, bool last) {
            s << CTypeCodeGenerator::codeGen(subtype);
            s << "_";
        });

        s << "tuple_t";

        return s.str();
    }
}
    