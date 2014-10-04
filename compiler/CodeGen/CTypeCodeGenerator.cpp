#include "CTypeCodeGenerator.h"

#include <sstream>

namespace Three {
    std::string CTypeCodeGenerator::codeGen(const NewDataType& type, const std::string& varName) {
        NewDataType usedType(type);

        std::string innerStr;

        // The check for a name catches aliased pointer types. Without doing this, we'll loose the
        // alias, and render the underlying type. That's not incorrect, but its much less readable,
        // and often surprising when inspecting the C code.
        while (usedType.isPointer() && !usedType.hasName()) {
            std::string pointerStr = "*";

            if (usedType.kind() == NewDataType::Kind::NullablePointer) {
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
            case NewDataType::Kind::Function:
            case NewDataType::Kind::CFunction:
                s << CTypeCodeGenerator::codeGenFunctionType(usedType, innerStr);
                break;
            default:
                s << CTypeCodeGenerator::codeGenType(usedType);
                s << innerStr;
                break;
        }

        return s.str();

    }

    std::string CTypeCodeGenerator::codeGenType(const NewDataType& type) {
        std::stringstream s;

        switch (type.kind()) {
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
            case NewDataType::Kind::CInt:
            case NewDataType::Kind::CChar:
            case NewDataType::Kind::CUnsignedInt:
            case NewDataType::Kind::Closure:
            case NewDataType::Kind::Structure:
            case NewDataType::Kind::Enumeration:
            case NewDataType::Kind::Union:
            case NewDataType::Kind::CStructure:
            case NewDataType::Kind::CStructPrefixedStructure:
                s << CTypeCodeGenerator::codeGenScalarType(type);
                break;
            case NewDataType::Kind::Array:
                break;
            case NewDataType::Kind::Pointer:
            case NewDataType::Kind::NullablePointer:
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

    std::string CTypeCodeGenerator::codeGenScalarType(const NewDataType& type) {
        std::stringstream s;

        if (CTypeCodeGenerator::typeCanBeConst(type)) {
            s << "const ";
        }

        if (type.isCScalar()) {
            s << CTypeCodeGenerator::codeGenCScalarType(type);
            return s.str();
        }

        switch (type.kind()) {
            case NewDataType::Kind::Void:
                s << "void";
                break;
            case NewDataType::Kind::Integer:
                s << "int";
                break;
            case NewDataType::Kind::Boolean:
                s << "bool";
                break;
            case NewDataType::Kind::Natural:
                if (type.widthSpecifier() > 16 && type.widthSpecifier() <= 32) {
                    s << "uint32_t";
                } else {
                    s << "unsigned int";
                }
                break;
            case NewDataType::Kind::Float:
            case NewDataType::Kind::Real:
                if (type.widthSpecifier() > 32) {
                    s << "double";
                } else {
                    s << "float";
                }
                break;
            case NewDataType::Kind::Character:
                s << "char";
                break;
            case NewDataType::Kind::Closure:
                s << "three_closure_t";
                break;
            case NewDataType::Kind::Vararg:
                s << "va_list";
                break;
            case NewDataType::Kind::Tuple:
                s << CTypeCodeGenerator::codeGenTupleStructName(type);
                break;
            case NewDataType::Kind::Structure:
            case NewDataType::Kind::Enumeration:
            case NewDataType::Kind::Union:
            case NewDataType::Kind::CStructure:
                s << type.name();
                break;
            case NewDataType::Kind::CStructPrefixedStructure:
                s << std::string("struct ") + type.name();
                break;
            default:
                assert(0 && "Bug: unhandled scalar type");
                break;
        }

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenCScalarType(const NewDataType& type) {
        assert(type.isCScalar());

        if (type.hasName()) {
            // This is a bit weird, but we need to preserve the type's name during codegen.
            // The actual underlying type will be correct here, but its way easier to understand
            // if we use any typedef'd name that was applied.
            return type.name();
        }

        switch (type.kind()) {
            case NewDataType::Kind::CUnsignedInt:
                return "unsigned int";
            case NewDataType::Kind::CInt:
                return "int";
            case NewDataType::Kind::CChar:
                return "char";
            default:
                break;
        }

        assert(0 && "Bug: unhandled scalar C type");

        return "";
    }

    std::string CTypeCodeGenerator::codeGenFunction(const NewDataType& type, const std::string& leadingString) {
        std::stringstream s;

        // returnType function(param1, param2)
        // returnType (*varName)(param1, param2)

        s << CTypeCodeGenerator::codeGen(type.returnType()) << " ";

        s << leadingString;

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
        }

        s << ")";

        return s.str();
    }

    std::string CTypeCodeGenerator::codeGenFunctionType(const NewDataType& type, const std::string& innerString) {
        std::stringstream s;

        // returnType (*varName)(param1, param2)

        s << "(" << innerString << ")";

        return CTypeCodeGenerator::codeGenFunction(type, s.str());
    }

    std::string CTypeCodeGenerator::codeGenTupleStructName(const NewDataType& type) {
        std::stringstream s;

        assert(type.kind() == NewDataType::Kind::Tuple);
        assert(type.subtypeCount() > 0);

        type.eachSubtypeWithLast([&] (const NewDataType& subtype, bool last) {
            s << CTypeCodeGenerator::codeGen(subtype);
            s << "_";
        });

        s << "tuple_t";

        std::string result = s.str();

        std::replace(result.begin(), result.end(), '*', 'P'); // replace all '*' to 'P'
        std::replace(result.begin(), result.end(), ' ', '_'); // replace all ' ' to '_'

        return result;
    }

    bool CTypeCodeGenerator::typeCanBeConst(const NewDataType& type) {
        switch (type.kind()) {
            case NewDataType::Kind::Void:
            case NewDataType::Kind::Vararg:
                return false;
            default:
                break;
        }

        return type.access() == NewDataType::Access::Read;
    }
}
    