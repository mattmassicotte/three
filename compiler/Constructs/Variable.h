#pragma once

#include "TypeReference.h"
#include "../CodeGen/CSource.h"

namespace Three {
    class Variable {
    public:
        Variable();
        Variable(const std::string& name, const TypeReference& type);

        void setName(const std::string& value);
        std::string name() const;
        void setType(const TypeReference& type);
        TypeReference type() const;

        void codeGenCSourceForDefinition(CSource* source);

        std::string _environment;

    private:
        std::string   _name;
        TypeReference _type;
    };
}
