#pragma once

#include "../ASTNode.h"

namespace Three {
    class ValueNode : public ASTNode {
    public:
        ValueNode(const std::string& value);

        std::string name() const;
        void accept(ASTVisitor& visitor);

        void setValue(const std::string& value);
        std::string value() const;

    private:
        std::string _value;
    };
}
