#pragma once

#include "BinaryOperatorNode.h"

namespace Three {
    class PlainBinaryOperatorNode : public BinaryOperatorNode {
    public:
        PlainBinaryOperatorNode(const std::string& type);

        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

    private:
        std::string _type;
    };
}
