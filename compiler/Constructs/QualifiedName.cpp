#include "QualifiedName.h"

#include <sstream>

namespace Three {
    QualifiedName::QualifiedName() {
    }

    QualifiedName::QualifiedName(const std::string& identifier) {
        _name = identifier;
    }

    std::string QualifiedName::to_s() const {
        std::stringstream s;

        for (const std::string& component : components) {
            s << component;

            // put a seperator in as long as there's a non-blank name
            if (_name != "") {
                s << "_3_";
            }
        }

        s << _name;

        return s.str();
    }

    std::string QualifiedName::name() const {
        return _name;
    }

    void QualifiedName::setName(const std::string& name) {
        assert(name != "");

        _name = name;
    }

    size_t QualifiedName::numberOfComponents() const {
        return components.size();
    }

    bool QualifiedName::valid() const {
        return this->numberOfComponents() > 0 || _name != "";
    }

    bool QualifiedName::namespaced() const {
        return components.size() > 0;
    }

    std::vector<std::string> QualifiedName::namespaceComponents() const {
        return components;
    }

    bool QualifiedName::sameNamespaceAs(const QualifiedName& other) const {
        if (!this->namespaced() || !other.namespaced()) {
            return false;
        }

        return this->components == other.components;
    }

    void QualifiedName::appendComponent(const std::string& path) {
        assert(path != "");

        components.push_back(path);
    }

    void QualifiedName::shiftLastComponentToName() {
        assert(this->name() == "");
        assert(components.size() > 0);

        this->setName(components.back());
        components.pop_back();
    }

    void QualifiedName::prependName(const QualifiedName& prefix) {
        components.insert(components.cbegin(), prefix.components.cbegin(), prefix.components.cend());
    }

    bool QualifiedName::operator==(const QualifiedName& other) const {
        if (_name != other.name()) {
            return false;
        }

        return this->components == other.components;
    }

    bool QualifiedName::operator!=(const QualifiedName& other) const {
        return !(*this == other);
    }
}
