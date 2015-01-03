#pragma once

#include <string>
#include <vector>

namespace Three {
    class QualifiedName {
    public:
        QualifiedName();
        QualifiedName(const std::string& identifier);
        std::string to_s() const;
        std::string name() const;
        void setName(const std::string& name);
        size_t numberOfComponents() const;
        bool valid() const;

        bool namespaced() const;
        std::vector<std::string> namespaceComponents() const;
        bool sameNamespaceAs(const QualifiedName& other) const;

        void appendComponent(const std::string& path);
        void shiftLastComponentToName();
        void prependName(const QualifiedName& prefix);

        bool operator ==(const QualifiedName& other) const;
        bool operator !=(const QualifiedName& other) const;

        std::vector<std::string> components;

    private:
        std::string _name;
    };
}
