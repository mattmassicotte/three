#pragma once

#include <string>

namespace Three {
    class Entity {
    public:
        virtual ~Entity();

        void setName(const std::string& value);
        std::string name() const;
        void setNamespace(const std::string& ns);
        std::string namespacePrefix() const;
        bool isNamespaced() const;

        virtual std::string fullyQualifiedName() const = 0;

    private:
        std::string _name;
        std::string _namespace;
    };
}
