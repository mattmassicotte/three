#pragma once

#include "../ASTNode.h"

namespace Language {
    class IntegerLiteralNode : public ASTNode {
    public:
        static IntegerLiteralNode* parse(Parser& parser);
        static uint64_t integerValue(const std::string& string);

    public:
        virtual std::string name() const;

        void setValue(uint64_t v);
        uint64_t value() const;

        void codeGenCSource(CSourceContext& context);

    private:
        uint64_t _value;
    };
}