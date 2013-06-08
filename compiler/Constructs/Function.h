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
        void setReturnType(const TypeReference& type);
        TypeReference returnType() const;

        void addParameter(const std::string& name, const TypeReference& type);
        uint32_t parameterCount() const ;
        Variable* parameterAtIndex(uint32_t index) const;
        void      eachParameterWithLast(std::function<void (Variable*, bool)> func) const;

        void codeGenCSource(CSourceContext& context, std::function<void (void)> func) const;

    private:
        std::string            _name;
        TypeReference          _returnType;
        std::vector<Variable*> _parameters;
    };
}
