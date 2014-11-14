#include "CTypeCodeGenerator.h"

#include <sstream>

namespace Three {
    std::string CTypeCodeGenerator::codeGen(const DataType& type, const std::string& varName) {
        DataType usedType(type);

        std::string innerStr;

        // The check for a name catches aliased pointer types. Without doing this, we'll loose the
        // alias, and render the underlying type. That's not incorrect, but its much less readable,
        // and often surprising when inspecting the C code.
        while (usedType.isPointer() && !usedType.hasName()) {
            std::string pointerStr = "*";

            if (usedType.kind() == DataType::Kind::NullablePointer) {
                pointerStr += " THREE_ATTR_NULLABLE";
            }

            if (CTypeCodeGenerator::typeCanBeConst(usedType)) {
                pointerStr += " const";
            }

            assert(usedType.subtypeCount() == 1 && "Error: a pointer must have exactly one subtype");
            usedType = usedType.subtypeAtIndex(0);

            // pre-pend the value, because we are working our way in
            innerStr = pointerStr + innerStr;
        }

        // apply the variable name here, if we have one
        if (varName.size() > 0) {
            innerStr += " " + varName;
        }

        std::stringstream s;

        switch (usedType.kind()) {
            case DataType::Kind::Function:
            case DataType::Kind::CFunction:
                s << CTypeCodeGenerator::codeGenFunctionType(usedType, innerStr);
                break;
            default:
                s << CTypeCodeGenerator::codeGenType(usedType);
                s << innerStr;
                break;
        }

        return s.str();

    }

    std::string CTypeCodeGenerator::codeGenType(const DataType& type) {
        std::stringstream s;

        switch (type.kind()) {
            case DataType::Kind::Undefined: assert(0 && "Bug: cannot codegen an undefined type");
            case DataType::Kind::Unknown:   assert(0 && "Bug: cannot codegen an unknown type");
            case DataType::Kind::Void:
            case DataType::Kind::Integer:
            case DataType::Kind::Boolean:
            case DataType::Kind::Natural:
            case DataType::Kind::Float:
            case DataType::Kind::Real:
            case DataType::Kind::Character:
            case DataType::Kind::Vararg:
            case DataType::Kind::Tuple:
            case DataType::Kind::CInt:
            case DataType::Kind::CChar:
            case DataType::Kind::CUnsignedInt:
            case DataType::Kind::Closure:
            case DataType::Kind::Structure:
            case DataType::Kind::Enumeration:
            case DataType::Kind::Union:
            case DataType::Kind::CStructure:
            case DataType::Kind::CStructPrefixedStructure:
                s << CTypeCodeGenerator::codeGenScalarType(type);
                break;
            case DataType::Kind::Array:
                break;
            case DataType::Kind::Pointer:
            case DataType::Kind::NullablePointer:
                // Normally, we don't take this path. But,
                // if a pointer is a named type (aliased), we want to use
                // that in the C source.
                assert(type.hasName());

                if (CTypeCodeGenerator::typeCanBeConst(type)) {
                    s << "const ";
                }

                s << type.name();
                break;
            default:
                std::cout << type.kind() << std::endl;
                assert(0 && "Error: unhandled C type");
                break;
        }

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenScalarType(const DataType& type) {
        std::stringstream s;

        if (CTypeCodeGenerator::typeCanBeConst(type)) {
            s << "const ";
        }

        if (type.isCScalar()) {
            s << CTypeCodeGenerator::codeGenCScalarType(type);
            return s.str();
        }

        switch (type.kind()) {
            case DataType::Kind::Void:
                s << "void";
                break;
            case DataType::Kind::Integer:
                s << "int";
                break;
            case DataType::Kind::Boolean:
                s << "bool";
                break;
            case DataType::Kind::Natural:
                if (type.widthSpecifier() > 16 && type.widthSpecifier() <= 32) {
                    s << "uint32_t";
                } else {
                    s << "unsigned int";
                }
                break;
            case DataType::Kind::Float:
            case DataType::Kind::Real:
                if (type.widthSpecifier() > 32) {
                    s << "double";
                } else {
                    s << "float";
                }
                break;
            case DataType::Kind::Character:
                s << "char";
                break;
            case DataType::Kind::Closure:
                s << "three_closure_t";
                break;
            case DataType::Kind::Vararg:
                s << "va_list";
                break;
            case DataType::Kind::Tuple:
                s << CTypeCodeGenerator::codeGenTupleStructName(type);
                break;
            case DataType::Kind::Structure:
            case DataType::Kind::Enumeration:
            case DataType::Kind::Union:
            case DataType::Kind::CStructure:
                s << type.name();
                break;
            case DataType::Kind::CStructPrefixedStructure:
                s << std::string("struct ") + type.name();
                break;
            default:
                assert(0 && "Bug: unhandled scalar type");
                break;
        }

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenCScalarType(const DataType& type) {
        assert(type.isCScalar());

        if (type.hasName()) {
            // This is a bit weird, but we need to preserve the type's name during codegen.
            // The actual underlying type will be correct here, but its way easier to understand
            // if we use any typedef'd name that was applied.
            return type.name();
        }

        switch (type.kind()) {
            case DataType::Kind::CUnsignedInt:
                return "unsigned int";
            case DataType::Kind::CInt:
                return "int";
            case DataType::Kind::CChar:
                return "char";
            default:
                break;
        }

        assert(0 && "Bug: unhandled scalar C type");

        return "";
    }

    std::string CTypeCodeGenerator::codeGenFunction(const DataType& type, const std::string& leadingString) {
        std::stringstream s;

        // returnType function(param1, param2)
        // returnType (*varName)(param1, param2)

        s << CTypeCodeGenerator::codeGen(type.returnType()) << " ";

        s << leadingString;

        s << "(";

        if (type.parameterCount() == 0) {
            s << "void";
        } else {
            type.eachParameterWithLast([&s] (const DataType& paramType, bool last) {
                s << CTypeCodeGenerator::codeGen(paramType, paramType.label());
                if (!last) {
                    s << ", ";
                }
            });
        }

        s << ")";

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenFunctionType(const DataType& type, const std::string& innerString) {
        std::stringstream s;

        // returnType (*varName)(param1, param2)

        s << "(" << innerString << ")";

        return CTypeCodeGenerator::codeGenFunction(type, s.str());
    }

    std::string CTypeCodeGenerator::codeGenTupleStructName(const DataType& type) {
        std::stringstream s;

        assert(type.kind() == DataType::Kind::Tuple);
        assert(type.subtypeCount() > 0);

        type.eachSubtypeWithLast([&] (const DataType& subtype, bool last) {
            s << CTypeCodeGenerator::codeGen(subtype);
            s << "_";
        });

        s << "tuple_t";

        std::string result = s.str();

        std::replace(result.begin(), result.end(), '*', 'P'); // replace all '*' to 'P'
        std::replace(result.begin(), result.end(), ' ', '_'); // replace all ' ' to '_'

        return result;
    }

    bool CTypeCodeGenerator::typeCanBeConst(const DataType& type) {
        switch (type.kind()) {
            case DataType::Kind::Void:
            case DataType::Kind::Vararg:
                return false;
            default:
                break;
        }

        return type.access() == DataType::Access::Read;
    }
}
    