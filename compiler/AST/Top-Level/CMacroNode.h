#pragma once

#include "../ASTNode.h"

namespace Three {
    class CMacroNode : public ASTNode {
    public:
        CMacroNode(const std::string& value);

        DataType dataType() const;
        std::string name() const;
        virtual std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        std::string value() const;

    private:
        std::string _value;
    };
}
