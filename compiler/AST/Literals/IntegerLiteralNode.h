#pragma once

#include "LiteralNode.h"

namespace Three {
    class IntegerLiteralNode : public LiteralNode {
    public:
        static IntegerLiteralNode* parse(Parser& parser);
        static uint64_t integerValue(const std::string& string);

    public:
        std::string nodeName() const;
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        void setValue(uint64_t v);
        uint64_t value() const;

    private:
        uint64_t _value;
    };
}
