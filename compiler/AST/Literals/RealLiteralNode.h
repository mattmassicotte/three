#pragma once

#include "LiteralNode.h"

namespace Three {
    class RealLiteralNode : public LiteralNode {
    public:
        static RealLiteralNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        void setValue(double v);
        double value() const;

    private:
        double _value;
    };
}
