#pragma once

#include "LiteralNode.h"

namespace Three {
    class CharacterLiteralNode : public LiteralNode {
    public:
        static CharacterLiteralNode* parse(Parser& parser);

    public:
        std::string nodeName() const;
        void accept(ASTVisitor& visitor);

        void setValue(const std::string& value);
        std::string value() const;

    private:
        std::string _value;
    };
}
