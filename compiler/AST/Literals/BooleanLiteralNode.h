#pragma once

#include "../ASTNode.h"

namespace Three {
    class BooleanLiteralNode : public ASTNode {
    public:
        static BooleanLiteralNode* parse(Parser& parser);

    public:
        std::string name() const;
        void accept(ASTVisitor& visitor);

        bool value() const;

        void codeGen(CSourceContext& context);

    private:
        bool _value;
    };
}
