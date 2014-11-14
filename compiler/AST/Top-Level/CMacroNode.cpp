#include "CMacroNode.h"

namespace Three {
    CMacroNode::CMacroNode(const std::string& value) : _value(value) {
    }

    DataType CMacroNode::dataType() const {
        return DataType(DataType::Kind::CUnspecifiedMacro);
    }

    std::string CMacroNode::name() const {
        return this->nodeName();
    }

    std::string CMacroNode::nodeName() const {
        return "C Macro";
    }

    void CMacroNode::accept(ASTVisitor& visitor) {
        visitor.visit(*this);
    }

    std::string CMacroNode::value() const {
        return _value;
    }
}
