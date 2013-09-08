#include "ValueNode.h"

namespace Three {
    ValueNode::ValueNode(const std::string& value) : _value(value) {
    }

    std::string ValueNode::name() const {
        return "Value";
    }

    void ValueNode::setValue(const std::string& value) {
        _value = value;
    }

    std::string ValueNode::value() const {
        return _value;
    }

    void ValueNode::codeGenCSource(CSourceContext& context) {
        context << this->value();
    }
}
