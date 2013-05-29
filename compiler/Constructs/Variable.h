#pragma once

#include "TypeReference.h"

namespace Language {
    class Variable {
    public:
        Variable();
        Variable(const std::string& name, const TypeReference& type);

        void setName(const std::string& value);
        std::string name() const;
        void setType(const TypeReference& type);
        TypeReference type() const;

    private:
        std::string   _name;
        TypeReference _type;
    };
}
