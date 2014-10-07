#include "QualifiedName.h"

#include <sstream>

namespace Three {
    QualifiedName::QualifiedName() {
    }

    QualifiedName::QualifiedName(const std::string& identifier) {
        components.push_back(identifier);
    }

    std::string QualifiedName::to_s() const {
        std::stringstream s;

        for (size_t i = 0; i < components.size(); ++i) {
            s << components.at(i);

            if (i < components.size() - 1) {
                s << "_3_";
            }
        }

        return s.str();
    }

    std::string QualifiedName::lastComponent() const {
        assert(this->numberOfComponents() > 0);

        return components.back();
    }

    size_t QualifiedName::numberOfComponents() const {
        return components.size();
    }

    bool QualifiedName::namespaced() const {
        return components.size() > 1;
    }

    std::vector<std::string> QualifiedName::namespaceComponents() const {
        std::vector<std::string> vector(components.cbegin(), components.cend() - 1);

        return vector;
    }

    void QualifiedName::appendComponent(const std::string& path) {
        components.push_back(path);
    }
    void QualifiedName::prependName(const QualifiedName& prefix) {
        components.insert(components.cbegin(), prefix.components.cbegin(), prefix.components.cend());
    }
}
