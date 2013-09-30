#pragma once

#include "Entity.h"
#include "TypeReference.h"
#include "Variable.h"
#include "../CodeGen/CSourceContext.h"

#include <vector>
#include <functional>

namespace Language {
    class Function : public Three::Entity {
    public:
        virtual ~Function();

        virtual std::string str() const;

        std::string fullyQualifiedName() const;

        void setReturnType(const TypeReference& type);
        TypeReference returnType() const;
        void setPseudoMethodType(const TypeReference& type);
        TypeReference pseudoMethodType() const;
        bool isPseudoMethod() const;
        DataType* createType() const;

        void addParameter(const std::string& name, const TypeReference& type);
        uint32_t parameterCount() const ;
        Variable* parameterAtIndex(uint32_t index) const;
        void      eachParameterWithLast(std::function<void (Variable*, bool)> func) const;

        void codeGenCSource(CSourceContext& context, std::function<void (void)> func) const;

    private:
        TypeReference          _returnType;
        TypeReference          _pseudoMethodType;
        std::vector<Variable*> _parameters;
    };
}
