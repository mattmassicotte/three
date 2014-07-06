#pragma once

#include "OperatorNode.h"

namespace Three {
    class DestructuredAssignmentOperatorNode : public OperatorNode {
    public:
        static DestructuredAssignmentOperatorNode* parse(Parser& parser, ASTNode* firstVariable);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);
    };
}
