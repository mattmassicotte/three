#include "Entity.h"

namespace Three {
    Entity::~Entity() {
    }

    void Entity::setName(const std::string& value) {
        _name = value;
    }

    std::string Entity::name() const {
        return _name;
    }

    void Entity::setNamespace(const std::string& ns) {
        _namespace = ns;
    }

    std::string Entity::namespacePrefix() const {
        return _namespace;
    }

    bool Entity::isNamespaced() const {
        return _namespace.length() > 0;
    }
}
