#pragma once

#include <string>
#include <vector>

namespace Three {
    class QualifiedName {
    public:
        QualifiedName();
        QualifiedName(const std::string& identifier);
        std::string to_s() const;
        std::string lastComponent() const;
        size_t numberOfComponents() const;

        bool namespaced() const;
        std::vector<std::string> namespaceComponents() const;

        void appendComponent(const std::string& path);
        void prependName(const QualifiedName& prefix);

        std::vector<std::string> components;
    };
}
