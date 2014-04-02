#pragma once

#include "LiteralNode.h"

namespace Three {
    class StringLiteralNode : public LiteralNode {
    public:
        static StringLiteralNode* parse(NewParser& parser);

    public:
        std::string nodeName() const;
        std::string name() const;
        std::string str() const;
        void accept(ASTVisitor& visitor);

        void setStringValue(const std::string& name);
        std::string stringValue() const;

    private:
        std::string _stringValue;
    };
}
