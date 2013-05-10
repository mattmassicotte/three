#pragma once

#include "ASTNode.h"

namespace Language {
    class IntegerLiteralNode : public ASTNode {
    public:
        static IntegerLiteralNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        void setValue(uint64_t v);
        uint64_t value() const;

        virtual void renderCCode(std::stringstream& stream);

    private:
        uint64_t _value;
    };
}
