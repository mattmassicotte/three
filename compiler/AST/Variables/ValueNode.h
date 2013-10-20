#pragma once

#include "../ASTNode.h"

namespace Three {
    class ValueNode : public ASTNode {
    public:
        ValueNode(const std::string& value);

        virtual std::string name() const;

        void setValue(const std::string& value);
        std::string value() const;

        void codeGen(CSourceContext& context);

    private:
        std::string _value;
    };
}