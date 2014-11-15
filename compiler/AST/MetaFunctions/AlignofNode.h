#pragma once

#include "MetafunctionNode.h"

namespace Three {
    class AlignofNode : public MetafunctionNode {
    public:
    static AlignofNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        void accept(ASTVisitor& visitor);

        DataType dataTypeArgument;
    	};
}
