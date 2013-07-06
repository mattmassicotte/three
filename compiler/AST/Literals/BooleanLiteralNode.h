#pragma once

#include "../ASTNode.h"

namespace Language {
    class BooleanLiteralNode : public ASTNode {
    public:
        static BooleanLiteralNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        bool value() const;

        void codeGenCSource(CSourceContext& context);

    private:
        bool _value;
    };
}
