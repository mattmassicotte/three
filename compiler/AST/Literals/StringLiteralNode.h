#pragma once

#include "../ASTNode.h"

namespace Three {
    class StringLiteralNode : public ASTNode {
    public:
        static StringLiteralNode* parse(Parser& parser);

    public:
        std::string name() const;
        std::string str() const;
        void accept(ASTVisitor& visitor);

        void setStringValue(const std::string& name);
        std::string stringValue() const;

        void codeGen(CSourceContext& context);

    private:
        std::string _stringValue;
    };
}
