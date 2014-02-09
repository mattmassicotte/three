#pragma once

#include "../ASTNode.h"

namespace Three {
    class IntegerLiteralNode : public ASTNode {
    public:
        static IntegerLiteralNode* parse(Parser& parser);
        static uint64_t integerValue(const std::string& string);

    public:
        virtual std::string name() const;
        void accept(ASTVisitor& visitor);

        void setValue(uint64_t v);
        uint64_t value() const;

        void codeGen(CSourceContext& context);

    private:
        uint64_t _value;
    };
}
