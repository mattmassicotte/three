#include "ValueNode.h"

namespace Three {
    ValueNode::ValueNode(const std::string& value, const TypeReference& type) : _value(value), _type(type) {
    }

    TypeReference ValueNode::nodeType() const {
        return _type;
    }

    std::string ValueNode::name() const {
        return "Value";
    }

    void ValueNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    void ValueNode::setValue(const std::string& value) {
        _value = value;
    }

    std::string ValueNode::value() const {
        return _value;
    }
}
