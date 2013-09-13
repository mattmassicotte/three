#pragma once

#include "TypeReference.h"
#include "Variable.h"
#include "../CodeGen/CSourceContext.h"

#include <vector>
#include <functional>

namespace Language {
    class Function {
    public:
        virtual ~Function();

        virtual std::string str() const;

        void setName(const std::string& value);
        std::string name() const;
        std::string fullyQualifiedName() const;
        void setNamespace(const std::string& ns);
        std::string namespacePrefix() const;
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
        std::string            _name;
        std::string            _namespace;
        TypeReference          _returnType;
        TypeReference          _pseudoMethodType;
        std::vector<Variable*> _parameters;
    };
}
