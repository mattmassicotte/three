#include "TypeReference.h"
#include "TranslationUnit.h"
#include "DataType.h"
#include "../CodeGen/CSourceContext.h"

#include <assert.h>

namespace Three {
    TypeReference TypeReference::ref(Three::Module* module, const std::string& name, uint32_t indirection) {
        assert(module);

        DataType* type = module->dataTypeForName(name);

        assert(type && "Type cannot be null");

        return TypeReference(type, indirection);
    }

    TypeReference::TypeReference(DataType* referencedType, uint32_t indirection, const std::vector<uint32_t>& dimensions) :
        _type(referencedType),
        _indirection(indirection),
        _arrayDimensions(dimensions) {
        // We have to store this in case the type changes the value.  This is a little bit of a 
        // hack to address self-referential struct definition codegen for C.
        if (_type) {
            _prependsStructKeyword = _type->cSourcePrependStructKeyword();
        }
    }

    std::string TypeReference::str() const {
        std::stringstream s;

        s << std::string(this->indirectionDepth(), '*');
        s << this->referencedType()->str();

        return s.str();
    }

    DataType* TypeReference::referencedType() const {
        return _type;
    }

    void TypeReference::setReferencedType(DataType* type) {
        _type = type;
    }

    std::string TypeReference::name() const {
        return this->referencedType()->fullyQualifiedName();
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

    bool TypeReference::isArray() const {
        return _arrayDimensions.size() > 0;
    }

    std::vector<uint32_t> TypeReference::arrayDimensions() const {
        return _arrayDimensions;
    }

    void TypeReference::codeGenCSourceFunctionType(CSource* source, const std::string& variableName) const {
        assert(!_prependsStructKeyword);

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
        _type->eachParameterWithLast([=] (const TypeReference& param, const std::string& name, bool last) {
            param.codeGenCSource(source, "");
            if (!last) {
                source->print(", ");
            }
        });

        source->print(")");
    }

    void TypeReference::codeGenCSource(CSource* source, const std::string& variableName) const {
        // It seems like it would make more sense to do this for both functions and
        // closures.  However, closures are actually structures, and nearly all of the time
        // they have to be referenced like regular structure types.  In rare cases,
        // like invoking their functions, this method should not be used.  But, in pretty much
        // all other situations, this check is appropriate.
        if (_type->flavor() == DataType::Flavor::Function) {
            this->codeGenCSourceFunctionType(source, variableName);
            return;
        }

        if (_prependsStructKeyword) {
            source->print("struct ");
        }

        source->print(_type->cSourceName());
        source->print(std::string(this->indirectionDepth(), '*'));

        if (variableName.size() > 0) {
            source->print(" ");
            source->print(variableName);
        }
    }

    void TypeReference::codeGen(CSourceContext& context, const std::string& variableName) const {
        // It seems like it would make more sense to do this for both functions and
        // closures.  However, closures are actually structures, and nearly all of the time
        // they have to be referenced like regular structure types.  In rare cases,
        // like invoking their functions, this method should not be used.  But, in pretty much
        // all other situations, this check is appropriate.
        if (_type->flavor() == DataType::Flavor::Function) {
            this->codeGenFunction(context, variableName);
            return;
        }

        if (_prependsStructKeyword) {
            context << "struct ";
        }

        context << _type->cSourceName();
        context << std::string(this->indirectionDepth(), '*');

        if (variableName.size() > 0) {
            context << " " << variableName;
        }
    }

    void TypeReference::codeGenFunction(CSourceContext& context, const std::string& variableName) const {
        assert(!_prependsStructKeyword);

        // return (*varName)(param1, param2);
        _type->returnType().codeGen(context);
        context << " (";

        // This is super-subtle.  It does not make sense to generate a function type without at least one star.
        context << std::string(std::max((uint32_t)1, this->indirectionDepth()), '*');

        if (variableName.size() > 0) {
            context << variableName;
        }

        context << ")(";

        // our environment pointer
        if (_type->flavor() == DataType::Flavor::Closure) {
            context << "void*";
            if (_type->childCount() > 0) {
                context << ", ";
            }
        }

        // parameters
        _type->eachParameterWithLast([&context] (const TypeReference& param, const std::string& name, bool last) {
            param.codeGen(context);
            if (!last) {
                context << ", ";
            }
        });

        context << ")";
    }
}
