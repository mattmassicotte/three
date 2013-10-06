#pragma once

#include "../ASTNode.h"

namespace Three {
    class BooleanLiteralNode : public ASTNode {
    public:
        static BooleanLiteralNode* parse(Parser& parser);

    public:
        virtual std::string name() const;

        bool value() const;

        void codeGen(CSourceContext& context);

    private:
        bool _value;
    };
}
