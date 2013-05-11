#pragma once

#include "ASTNode.h"

namespace Language {
    class StringLiteralNode : public ASTNode {
    public:
        static StringLiteralNode* parse(Parser& parser);

    public:
        virtual std::string name() const;
        std::string str() const;

        void setStringValue(const std::string& name);
        std::string stringValue() const;

        virtual void renderCCode(std::stringstream& stream, uint32_t indentation);

    private:
        std::string _stringValue;
    };
}
